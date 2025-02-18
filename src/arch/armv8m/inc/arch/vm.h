/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __ARCH_VM_H__
#define __ARCH_VM_H__

#include <bao.h>
#include <arch/csfrs.h>
#include <cpu.h>
#include <arch/interrupts.h>
#include <emul.h>
#include <arch/csa.h>
#include <arch/vfp.h>
#include <arch/sysregs.h>
#include <timer.h>
#include <arch/vtimer.h>
#include <arch/vmpu.h>
#include <arch/vnvic.h>

#define MAX_OF_GP_REGS (sizeof(union gp_regs) / sizeof(unsigned long))

struct vnvic_src {
    node_t node;
    struct vcpu* owner;
    spinlock_t lock;
    irqid_t id;
    uint8_t prio;
    uint32_t target;
    bool enabled;
};

struct vnvic_dscrp {
    paddr_t int_addr;
    paddr_t src_addr;
};

struct arch_vm_platform {
    /* interrupt controller */
    struct vnvic_dscrp vir;
};

struct vm_arch {
    /* interrupt controller */
    /* TODO needed? struct vnvic_int vnvic_int; */
    struct vnvic_src* vnvic_src;

    paddr_t vnvic_int_addr;
    spinlock_t vnvic_int_lock;
    paddr_t vnvic_src_addr;
    spinlock_t vnvic_src_lock;

    struct emul_mem vnvic_int_emul;
    struct emul_mem vnvic_src_emul;
};

struct vcpu_arch {
    vcpuid_t core_id;
    struct vtimer vtimer;
    struct vmpu vmpu;
    struct vnvic vnvic;
    struct {
        BITMAP_ALLOC(bitmap, SAU_ARCH_MAX_NUM_ENTRIES);
        /**
         * A locked region means that it can never be removed from the MPU. For example,
         */
        BITMAP_ALLOC(locked, SAU_ARCH_MAX_NUM_ENTRIES);
    } sau_vm;
};

struct special_regs {
    unsigned long msp;
    unsigned long psp;
    unsigned long msp_lim;
    unsigned long psp_lim;
    unsigned long basepri;
    unsigned long primask;
    unsigned long faultmask;
    unsigned long control;
    unsigned long xpsr;
};

struct arch_regs {
    union gp_regs {
        unsigned long r[16];
        struct {
            unsigned long r0;
            unsigned long r1;
            unsigned long r2;
            unsigned long r3;
            unsigned long r4;
            unsigned long r5;
            unsigned long r6;
            unsigned long r7;
            unsigned long r8;
            unsigned long r9;
            unsigned long r10;
            unsigned long r11;
            unsigned long r12;
            unsigned long sp; /* r13 */
            unsigned long lr; /* r14 */
            unsigned long pc; /* r15 */
        };
    } gp_regs;
    // R/W sysregs from SCB
    unsigned long icsr;
    unsigned long vtor;
    unsigned long aircr;
    unsigned long scr;
    unsigned long ccr;
    unsigned long shpr1;
    unsigned long shpr2;
    unsigned long shpr3;
    unsigned long shcsr;
    unsigned long mmfar;
    unsigned long bfar;
    unsigned long csselr;
    unsigned long cpacr;
    // R/W sysregs from DHCSR
    unsigned long dhcsr;
    unsigned long dcrdr;
    unsigned long demcr;
    unsigned long dauthctrl;

    struct special_regs sp_regs;
    struct vfp vfp_regs;

} __attribute__((__packed__, aligned(sizeof(unsigned long))));

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

void vnvic_vcpu_init(struct vcpu* vcpu);

void nvic_config_irq(irqid_t int_id, bool en);

struct vm;
void nvic_assign_int_to_vm(struct vm* vm, irqid_t id);

void nvic_assign_int_to_vm(struct vm* vm, irqid_t id);

#endif /* __ARCH_VM_H__ */
