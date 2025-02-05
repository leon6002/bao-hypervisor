/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <vm.h>
#include <arch/sysregs.h>
#include <fences.h>
#include <string.h>
#include <config.h>

void vm_arch_init(struct vm* vm, const struct vm_config* vm_config)
{
    UNUSED_ARG(vm);
    UNUSED_ARG(vm_config);
}

void vcpu_arch_init(struct vcpu* vcpu, struct vm* vm)
{
    UNUSED_ARG(vcpu);
    UNUSED_ARG(vm);
}

void vcpu_arch_reset(struct vcpu* vcpu, vaddr_t entry)
{
    memset(&vcpu->regs, 0, sizeof(struct arch_regs));

    // TODO:ARMV8-M - to we need replicate the warm reset sequence? (TakeReset - page 1919)
    // msp = vector[0]
    // If so, we do this:
    // Set gp and sp regs to reset values
    vcpu->regs.lr = entry;
    vcpu->regs.sp_regs.xpsr = xPSR_RESET_VALUE;

    vfp_reset(&vcpu->regs.vfp_regs);
}

bool vcpu_arch_is_on(struct vcpu* vcpu)
{
    UNUSED_ARG(vcpu);

    return true;
}

unsigned long vcpu_readreg(struct vcpu* vcpu, unsigned long reg)
{
    (void)vcpu;
    (void)reg;
    return 0;
}

void vcpu_writereg(struct vcpu* vcpu, unsigned long reg, unsigned long val)
{
    (void)vcpu;
    (void)reg;
    (void)val;
}

void vcpu_restore_state(struct vcpu *vcpu)
{
    UNUSED_ARG(vcpu);
    // csrs_vsstatus_write(vcpu->regs.vsstatus);
    // csrs_vstvec_write(vcpu->regs.vstvec);
    // csrs_vsscratch_write(vcpu->regs.vsscratch);
    // csrs_vsepc_write(vcpu->regs.vsepc);
    // csrs_vscause_write(vcpu->regs.vscause);
    // csrs_vstval_write(vcpu->regs.vstval);
    // csrs_vsatp_write(vcpu->regs.vsatp);
    // if (CPU_HAS_EXTENSION(CPU_EXT_SSTC)) {
    //     csrs_vstimecmp_write(vcpu->regs.vstimecmp);
    // }

    // csrs_hie_write(vcpu->regs.hie);
    // csrs_hvip_write(vcpu->regs.hvip);
    // csrs_hgatp_write(vcpu->vm->arch.hgatp);

    // timer_event_add(&vcpu->arch.timer_event);
    // vfp_restore_state(&vcpu->regs.vfp);
}

void vcpu_save_state(struct vcpu* vcpu)
{
    UNUSED_ARG(vcpu);
    // vcpu->regs.vsstatus = csrs_vsstatus_read();
    // vcpu->regs.vstvec = csrs_vstvec_read();
    // vcpu->regs.vsscratch = csrs_vsscratch_read();
    // vcpu->regs.vsepc = csrs_vsepc_read();
    // vcpu->regs.vscause = csrs_vscause_read();
    // vcpu->regs.vstval = csrs_vstval_read();
    // vcpu->regs.vsatp = csrs_vsatp_read();
    // if (CPU_HAS_EXTENSION(CPU_EXT_SSTC)) {
    //     vcpu->regs.vstimecmp = csrs_vstimecmp_read();
    // }

    // vcpu->regs.hie  = csrs_hie_read();
    // vcpu->regs.hvip = csrs_hvip_read();

    // timer_event_remove(&vcpu->arch.timer_event);
    // vfp_save_state(&vcpu->regs.vfp);
}
