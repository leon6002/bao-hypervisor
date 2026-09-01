/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <vbootctrl.h>
#include <arch/emul.h>
#include <bao.h>
#include <srs.h>

static bool vbootctrl_emul_handler(struct emul_access* acc)
{
    struct vcpu* vcpu = cpu()->vcpu;
    unsigned long notify = 0;
    struct vcpu* waking_vcpu = NULL;

    if (acc->addr != platform.arch.bootctrl_addr && acc->width != 4) {
        /* debug probe: guest trace markers (16-bit writes into the page) */
        if (acc->write) {
            console_printk("guest-trace: 0x%lx c%d\n", vcpu_readreg(vcpu, acc->reg),
                (int)cpu()->id);
        }
        /* ignore access */
        if (!acc->write && !emul_arch_is_bwop(&acc->arch)) {
            vcpu_writereg(vcpu, acc->reg, 0);
        }
        return true;
    }

    if (acc->write) {
        unsigned long val = vcpu_readreg(vcpu, acc->reg);
        for (size_t i = 0; i < vcpu->vm->cpu_num; i++) {
            if ((1U << i) & val) {
                waking_vcpu = vm_get_vcpu(vcpu->vm, i);
                if (waking_vcpu == NULL) {
                    continue;
                }
                if (!waking_vcpu->arch.started) {
                    notify |= (1UL << waking_vcpu->phys_id);
                    waking_vcpu->arch.started = true;
                }
            }
        }
    } else {
        unsigned long val = 0;
        for (size_t i = 0; i < vcpu->vm->cpu_num; i++) {
            struct vcpu* awake_vcpu = vm_get_vcpu(vcpu->vm, i);
            if (awake_vcpu == NULL) {
                continue;
            }
            if (awake_vcpu->arch.started) {
                val |= 1UL << i;
            }
        }
        vcpu_writereg(vcpu, acc->reg, val);
    }

    if (acc->write) {
        INFO("vbootctrl: wr pc=0x%lx cpu%d notify=0x%lx eipswh=0x%lx gmpeid=%d\n",
            vcpu_readpc(cpu()->vcpu), (int)cpu()->id, notify,
            srs_eipswh_read(), (int)srs_gmpeid_read());
    }

    /* Notify physical CPUs, if any */
    if (notify != 0) {
        for (cpuid_t c = 0; c < platform.cpu_num; c++) {
            if (notify & (1UL << c)) {
                interrupts_cpu_sendipi(c);
            }
        }
    }

    return true;
}

void vbootctrl_init(struct vm* vm)
{
    if (cpu()->id == vm->master) {
        vm->arch.bootctrl_emul = (struct emul_mem){
            .va_base = platform.arch.bootctrl_addr,
            /* BOOTCTRL is 4 bytes only */
            .size = ALIGN(4, PAGE_SIZE),
            .handler = vbootctrl_emul_handler,
        };
        vm_emul_add_mem(vm, &vm->arch.bootctrl_emul);
    }
}
