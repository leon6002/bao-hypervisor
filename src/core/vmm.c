/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <vmm.h>
#include <vm.h>
#include <config.h>
#include <cpu.h>
#include <spinlock.h>
#include <fences.h>
#include <string.h>
#include <shmem.h>

static struct vm_assignment {
    spinlock_t lock;
    struct cpu_synctoken root_sync;
    bool master;
    size_t ncpus;
    cpumap_t cpus;
    struct vm_allocation vm_alloc;
    struct vm_install_info vm_install_info;
    volatile bool install_info_ready;
} vm_assign[CONFIG_VM_NUM];

static bool vmm_assign_vcpu(bool* master, vmid_t* vm_id)
{
    bool assigned = false;
    *master = false;
    /* Assign cpus according to vm affinity. */
    for (size_t i = 0; i < config.vmlist_size && !assigned; i++) {
        if (config.vmlist[i].cpu_affinity & (1UL << cpu()->id)) {
            spin_lock(&vm_assign[i].lock);
            if (!vm_assign[i].master) {
                vm_assign[i].master = true;
                vm_assign[i].ncpus++;
                vm_assign[i].cpus |= (1UL << cpu()->id);
                *master = true;
                assigned = true;
                *vm_id = i;
            } else if (vm_assign[i].ncpus < config.vmlist[i].platform.cpu_num) {
                assigned = true;
                vm_assign[i].ncpus++;
                vm_assign[i].cpus |= (1UL << cpu()->id);
                *vm_id = i;
            }
            spin_unlock(&vm_assign[i].lock);
        }
    }

    cpu_sync_barrier(&cpu_glb_sync);

    /* Assign remaining cpus not assigned by affinity. */
    if (assigned == false) {
        for (size_t i = 0; i < config.vmlist_size && !assigned; i++) {
            spin_lock(&vm_assign[i].lock);
            if (vm_assign[i].ncpus < config.vmlist[i].platform.cpu_num) {
                if (!vm_assign[i].master) {
                    vm_assign[i].master = true;
                    vm_assign[i].ncpus++;
                    *master = true;
                    assigned = true;
                    vm_assign[i].cpus |= (1UL << cpu()->id);
                    *vm_id = i;
                } else {
                    assigned = true;
                    vm_assign[i].ncpus++;
                    vm_assign[i].cpus |= (1UL << cpu()->id);
                    *vm_id = i;
                }
            }
            spin_unlock(&vm_assign[i].lock);
        }
    }

    return assigned;
}

static bool vmm_alloc_vm(struct vm_allocation* vm_alloc, struct vm_config* vm_config)
{
    /**
     * We know that we will allocate a block aligned to the PAGE_SIZE, which is guaranteed to
     * fulfill the alignment of all types. However, to guarantee the alignment of all fields, when
     * we calculate the size of a field in the vm_allocation struct, we must align the previous
     * total size calculated until that point, to the alignment of the type of the next field.
     */

    size_t total_size = sizeof(struct vm);
    size_t vcpus_offset = ALIGN(total_size, _Alignof(struct vcpu));
    total_size = vcpus_offset + (vm_config->platform.cpu_num * sizeof(struct vcpu));
    total_size = ALIGN(total_size, PAGE_SIZE);

    void* allocation = mem_alloc_page(NUM_PAGES(total_size), SEC_HYP_VM, MEM_ALIGN_NOT_REQ);
    if (allocation == NULL) {
        return false;
    }
    memset((void*)allocation, 0, total_size);

    vm_alloc->base = (vaddr_t)allocation;
    vm_alloc->size = total_size;
    vm_alloc->vm = (struct vm*)vm_alloc->base;
    vm_alloc->vcpus = (struct vcpu*)(vm_alloc->base + vcpus_offset);

    return true;
}

static struct vm_allocation* vmm_alloc_install_vm(vmid_t vm_id, bool master)
{
    struct vm_allocation* vm_alloc = &vm_assign[vm_id].vm_alloc;
    struct vm_config* vm_config = &config.vmlist[vm_id];
    if (master) {
        if (!vmm_alloc_vm(vm_alloc, vm_config)) {
            ERROR("Failed to allocate vm internal structures\n");
        }
        vm_assign[vm_id].vm_install_info = vmm_get_vm_install_info(vm_alloc);
        fence_ord_write();
        vm_assign[vm_id].install_info_ready = true;
    } else {
        while (!vm_assign[vm_id].install_info_ready) { }
        fence_ord_read();
        vmm_vm_install(&vm_assign[vm_id].vm_install_info);
    }

    return vm_alloc;
}

void vmm_init()
{
    vmm_arch_init();

    if (cpu_is_master()) {
        extern uint32_t plat_resf_at_boot;
        console_printk("RESF at boot = 0x%x\n", (unsigned)plat_resf_at_boot);
    }

    /* debug probe: walk the guest Port PIBC addresses from the hypervisor
     * to find which access hangs the bus (round-10 guest death site) */
    if (cpu_is_master()) {
        vaddr_t pw = mem_alloc_map_dev(&cpu()->as, SEC_HYP_GLOBAL, INVALID_VA,
            0xFFD90000, NUM_PAGES(0x7000));
        if (pw != INVALID_VA) {
            unsigned long grp[] = { 0, 1, 2, 3, 5, 6, 10, 17, 20, 21, 22, 24 };
            for (size_t i = 0; i < sizeof(grp) / sizeof(grp[0]); i++) {
                unsigned long off = 0x4000 + 0x40 * grp[i];
                console_printk("probe PIBC g%d @0x%lx : ", (int)grp[i],
                    0xFFD90000 + off);
                unsigned short v = *(volatile unsigned short*)(pw + off);
                console_printk("0x%x\n", (unsigned)v);
            }
            for (unsigned long g = 0; g <= 4; g++) {
                unsigned long off = 0xC80 + 0x40 * g + 0x4000;
                console_printk("probe APIBC%d @0x%lx : ", (int)g, 0xFFD90000 + off);
                unsigned short v = *(volatile unsigned short*)(pw + off);
                console_printk("rd=0x%x ", (unsigned)v);
                *(volatile unsigned short*)(pw + off) = v;
                console_printk("wr ok\n");
            }
            {
                volatile unsigned short* r =
                    (volatile unsigned short*)(pw + 0xC80 + 0x40 * 4 + 0x4000);
                console_printk("probe APIBC4 value-write 0x5 : ");
                *r = 0x0005;
                console_printk("ok rb=0x%x, restoring : ", (unsigned)*r);
                *r = 0x0000;
                console_printk("ok\n");
            }
            console_printk("probe done\n");
        }
    }
    vmm_io_init();
    shmem_init();
    remio_init();

    if (cpu_is_master()) {
        for (size_t i = 0; i < CONFIG_VM_NUM; i++) {
            vm_assign[i].lock = SPINLOCK_INITVAL;
            cpu_sync_init(&vm_assign[i].root_sync, config.vmlist[i].platform.cpu_num);
        }
    }

    cpu_sync_barrier(&cpu_glb_sync);

    bool master = false;
    vmid_t vm_id = INVALID_VMID;
    if (vmm_assign_vcpu(&master, &vm_id)) {
        struct vm_allocation* vm_alloc = vmm_alloc_install_vm(vm_id, master);
        struct vm_config* vm_config = &config.vmlist[vm_id];
        struct vm* vm = vm_init(vm_alloc, &vm_assign[vm_id].root_sync, vm_config, master, vm_id);
        cpu_sync_barrier(&vm->sync);
        vcpu_run(cpu()->vcpu);
    } else {
        cpu_powerdown();
    }
}
