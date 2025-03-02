/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __ARCH_VM_H__
#define __ARCH_VM_H__

#include <bao.h>
#include <arch/srs.h>
#include <cpu.h>
#include <arch/interrupts.h>
#include <emul.h>
#include <arch/vmpu.h>

#define MAX_OF_GP_REGS (sizeof(union gp_regs) / sizeof(unsigned long))

struct intc_dscrp {
    paddr_t intc1_addr;
    paddr_t intc2_addr;
};

struct arch_vm_platform {
    /* interrupt controller */
    struct intc_dscrp vir;
};

struct vm_arch {
    /* interrupt controller */
    size_t hello;
};

struct vcpu_arch {
    vcpuid_t core_id;
    struct vmpu vmpu;
};

struct arch_regs {
    union gp_regs {
        unsigned long r[10];
        struct {
            // r0-r3, r12 are callee-saved, and they are sanitized in the first execution on the
            // vcpu_arch_vm_entry
            unsigned long r4;
            unsigned long r5;
            unsigned long r6;
            unsigned long r7;
            unsigned long r8;
            unsigned long r9;
            unsigned long r10;
            unsigned long r11;
            unsigned long lr; /* r14 */
            // r15 is used for the first vm entry
            unsigned long pc; /* r15 */
        };
    } gp_regs;
};

void vcpu_arch_entry(void);

static inline void vcpu_arch_inject_hw_irq(struct vcpu* vcpu, irqid_t id)
{
    (void)vcpu;
    (void)id;
    /* virqc_inject(vcpu, id); */
}

static inline void vcpu_arch_inject_irq(struct vcpu* vcpu, irqid_t id)
{
    (void)vcpu;
    (void)id;
    /* virqc_inject(vcpu, id); */
}

#endif /* __ARCH_VM_H__ */
