/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <vm.h>
#include <arch/sysregs.h>
#include <fences.h>
#include <string.h>
#include <config.h>
#include <list.h>
#include <platform.h>
#include <arch/vtimer.h>
#include <arch/vmpu.h>
#include <arch/vnvic.h>

#define ESF_LR_EXEC_RETURN_VALUE (0x00000000)
#define ESF_xPSR_VALUE           (0x01000000)

struct e_stack_frame {
    uint32_t xPSR;
    uint32_t pc; // Return Address
    uint32_t lr; // LR (EXC_RETURN)
    uint32_t r12;
    uint32_t r3;
    uint32_t r2;
    uint32_t r1;
    uint32_t r0;

    // TODO:ARMV8M - We are missing here FP context
};

void vm_arch_init(struct vm* vm, const struct vm_config* vm_config)
{
    UNUSED_ARG(vm);
    UNUSED_ARG(vm_config);
}

void vcpu_arch_init(struct vcpu* vcpu, struct vm* vm)
{
    // Prepare exception stack frame for the first jump via exception return
    struct e_stack_frame frame = {
        .xPSR = ESF_xPSR_VALUE,
        .pc = vm->config->entry,
        .lr = ESF_LR_EXEC_RETURN_VALUE,
        .r12 = 0,
        .r3 = 0,
        .r2 = 0,
        .r1 = 0,
        .r0 = 0,
    };

    // TODO:ARMV8M - This can be logically moved to some later stage of vm_init
    //  Find a writable memory region to write the stack frame
    for (size_t j = 0; j < platform.region_num; j++) {
        struct mem_region* reg_plat = &platform.regions[j];
        if (reg_plat->perms == RWX) {
            for (size_t i = 0; i < vm->config->platform.region_num; i++) {
                struct vm_mem_region* reg_vm = &vm->config->platform.regions[i];
                if (reg_plat->base == reg_vm->base) {
                    // Set the virtual sp to the end of the region
                    vcpu->regs.sp_regs.msp =
                        (reg_vm->base + reg_vm->size) - sizeof(struct e_stack_frame);
                    // Copy the stack frame to the stack
                    memcpy((void*)(vcpu->regs.gp_regs.sp - sizeof(struct e_stack_frame)), &frame,
                        sizeof(struct e_stack_frame));
                    break;
                }
            }
        }
    }
}

void vcpu_arch_reset(struct vcpu* vcpu, vaddr_t entry)
{
    UNUSED_ARG(entry);

    // TODO:ARMV8-M - to we need replicate the warm reset sequence? (TakeReset - page 1919)
    // msp = vector[0]
    // If so, we do this:
    // Set gp regs to reset values (except regs from exception stack frame r0-r3, r12, sp, lr, pc)
    vcpu->regs.gp_regs.r4 = 0;
    vcpu->regs.gp_regs.r5 = 0;
    vcpu->regs.gp_regs.r6 = 0;
    vcpu->regs.gp_regs.r7 = 0;
    vcpu->regs.gp_regs.r8 = 0;
    vcpu->regs.gp_regs.r9 = 0;
    vcpu->regs.gp_regs.r10 = 0;
    vcpu->regs.gp_regs.r11 = 0;
    // Set sp regs to reset values (except regs from exception stack frame xPSR, msp)
    vcpu->regs.sp_regs.psp = 0;
    vcpu->regs.sp_regs.msp_lim = 0;
    vcpu->regs.sp_regs.psp_lim = 0;
    vcpu->regs.sp_regs.basepri = 0;
    vcpu->regs.sp_regs.primask = 0;
    vcpu->regs.sp_regs.faultmask = 0;
    vcpu->regs.sp_regs.control = 0;

    vfp_reset(&vcpu->regs.vfp_regs);
    vnvic_reset();
    vtimer_reset(&vcpu->arch.vtimer);
    vmpu_reset(&vcpu->arch.vmpu);
}

bool vcpu_arch_is_on(struct vcpu* vcpu)
{
    UNUSED_ARG(vcpu);

    return true;
}

unsigned long vcpu_readreg(struct vcpu* vcpu, unsigned long reg)
{
    if ((reg <= 0) || (reg > MAX_OF_GP_REGS)) {
        return 0;
    }
    return vcpu->regs.gp_regs.r[reg - 1];
}

void vcpu_writereg(struct vcpu* vcpu, unsigned long reg, unsigned long val)
{
    if ((reg <= 0) || (reg > MAX_OF_GP_REGS)) {
        return;
    }
    vcpu->regs.gp_regs.r[reg - 1] = val;
}

unsigned long vcpu_readpc(struct vcpu* vcpu)
{
    return vcpu->regs.gp_regs.pc;
}

void vcpu_writepc(struct vcpu* vcpu, unsigned long pc)
{
    vcpu->regs.gp_regs.pc = pc;
}

void vcpu_restore_state(struct vcpu* vcpu)
{
    scb_ns->icsr = vcpu->regs.icsr;
    scb_ns->vtor = vcpu->regs.vtor;
    scb_ns->aircr = vcpu->regs.aircr;
    scb_ns->scr = vcpu->regs.scr;
    scb_ns->ccr = vcpu->regs.ccr;
    scb_ns->shpr1 = vcpu->regs.shpr1;
    scb_ns->shpr2 = vcpu->regs.shpr2;
    scb_ns->shpr3 = vcpu->regs.shpr3;
    scb_ns->shcsr = vcpu->regs.shcsr;
    scb_ns->mmfar = vcpu->regs.mmfar;
    scb_ns->bfar = vcpu->regs.bfar;
    scb_ns->csselr = vcpu->regs.csselr;
    scb_ns->cpacr = vcpu->regs.cpacr;
    dcb_ns->dhcsr = vcpu->regs.dhcsr;
    dcb_ns->dcrdr = vcpu->regs.dcrdr;
    dcb_ns->demcr = vcpu->regs.demcr;
    dcb_ns->dauthctrl = vcpu->regs.dauthctrl;

    vnvic_restore_state(&vcpu->arch.vnvic, vcpu->vm->interrupt_bitmap);
    vtimer_restore_state(&vcpu->arch.vtimer);
    vmpu_restore_state(&vcpu->arch.vmpu);
    vfp_restore_state(&vcpu->regs.vfp_regs);
}

void vcpu_save_state(struct vcpu* vcpu)
{
    vcpu->regs.icsr = scb_ns->icsr;
    vcpu->regs.vtor = scb_ns->vtor;
    vcpu->regs.aircr = scb_ns->aircr;
    vcpu->regs.scr = scb_ns->scr;
    vcpu->regs.ccr = scb_ns->ccr;
    vcpu->regs.shpr1 = scb_ns->shpr1;
    vcpu->regs.shpr2 = scb_ns->shpr2;
    vcpu->regs.shpr3 = scb_ns->shpr3;
    vcpu->regs.shcsr = scb_ns->shcsr;
    vcpu->regs.mmfar = scb_ns->mmfar;
    vcpu->regs.bfar = scb_ns->bfar;
    vcpu->regs.csselr = scb_ns->csselr;
    vcpu->regs.cpacr = scb_ns->cpacr;
    vcpu->regs.dhcsr = dcb_ns->dhcsr;
    vcpu->regs.dcrdr = dcb_ns->dcrdr;
    vcpu->regs.demcr = dcb_ns->demcr;
    vcpu->regs.dauthctrl = dcb_ns->dauthctrl;

    vnvic_save_state(&vcpu->arch.vnvic, vcpu->vm->interrupt_bitmap);
    vtimer_save_state(&vcpu->arch.vtimer);
    vmpu_save_state(&vcpu->arch.vmpu);
    vfp_save_state(&vcpu->regs.vfp_regs);
}
