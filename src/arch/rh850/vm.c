/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <arch/srs.h>
#include <vm.h>
#include <arch/srs.h>
#include <fences.h>
#include <string.h>
#include <config.h>
#include <list.h>
#include <platform.h>
#include <arch/vmpu.h>
#include <mem.h>
#include <arch/vintc.h>

void vm_arch_init(struct vm* vm, const struct vm_config* vm_config)
{
    /* /1* TODO these registers must be initialized *1/ */
    /* // Basic system registers */
    /* unsigned long EIPC; */
    /* unsigned long FEPC; */
    /* unsigned long CTPC; */
    /* unsigned long EIWR; */
    /* unsigned long FEWR; */
    /* unsigned long EBASE; */
    /* unsigned long INTBP; */
    /* unsigned long MEA; */
    /* unsigned long MEI; */
    /* unsigned long RBIP; */

    /* // FPU system registers */
    /* unsigned long FPSR; */
    /* unsigned long FPEPC; */
    /* unsigned long FPST; */
    /* unsigned long FPCC; */

    /* // MPU function registers */
    /* unsigned long MCA; */
    /* unsigned long MCS; */
    /* unsigned long MCR; */
    /* unsigned long MPLA; */
    /* unsigned long MPUA; */
    /* unsigned long MPAT; */
    /* unsigned long MPIDn; */
    /* unsigned long MCI; */

    /* // Cache operation function registers */
    /* unsigned long ICTAGL; */
    /* unsigned long ICTAGH; */
    /* unsigned long ICDATL; */
    /* unsigned long ICDATH; */
    /* unsigned long ICERR; */

    /* // Guest Context Registers */
    /* unsigned long GMEIPC; */
    /* unsigned long GMFEPC; */
    /* unsigned long GMEBASE; */
    /* unsigned long GMINTBP; */
    /* unsigned long GMEIWR; */
    /* unsigned long GMFEWR; */
    /* unsigned long GMMEA; */
    /* unsigned long GMMEI; */
}

void vcpu_arch_init(struct vcpu* vcpu, struct vm* vm)
{
    vintc_init(vcpu);
    set_gmpeid(vcpu->id);
}

void vcpu_arch_reset(struct vcpu* vcpu, vaddr_t entry)
{
    memset(&vcpu->regs, 0, sizeof(struct arch_regs));
    set_eipc(entry);
}

bool vcpu_arch_is_on(struct vcpu* vcpu)
{
    return true;
}

unsigned long vcpu_readreg(struct vcpu* vcpu, unsigned long reg)
{
    if (reg > 32) {
        ERROR("reading register out of bounds");
    }

    return vcpu->regs.gp_regs.r[reg];
}

void vcpu_writereg(struct vcpu* vcpu, unsigned long reg, unsigned long val)
{
    if (reg > 32) {
        ERROR("writing register out of bounds");
    }

    vcpu->regs.gp_regs.r[reg] = val;
}

unsigned long vcpu_readpc(struct vcpu* vcpu)
{
    return vcpu->regs.pc;
}

void vcpu_writepc(struct vcpu* vcpu, unsigned long val)
{
    vcpu->regs.pc = val;
}

void vcpu_restore_state(struct vcpu* vcpu)
{
    ERROR("%s not implemented", __func__);
}

void vcpu_save_state(struct vcpu* vcpu)
{
    ERROR("%s not implemented", __func__);
}
