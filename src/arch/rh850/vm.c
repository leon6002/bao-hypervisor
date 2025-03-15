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

void vm_arch_init(struct vm* vm, const struct vm_config* vm_config) { }

void vcpu_arch_init(struct vcpu* vcpu, struct vm* vm)
{
    vintc_init(vcpu);
    set_gmpeid(vcpu->id);
    set_gmspid(vm->id + 1);
}

void vcpu_arch_reset(struct vcpu* vcpu, vaddr_t entry)
{
    memset(&vcpu->regs, 0, sizeof(struct arch_regs));
    set_eipc(entry);

    struct vm* vm = vcpu->vm;

    set_gmpeid(vcpu->id);
    set_gmspid(vm->id + 1);
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
