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
#include <arch/vmpu.h>
#include <mem.h>

void vm_arch_init(struct vm* vm, const struct vm_config* vm_config) { }

void vcpu_arch_init(struct vcpu* vcpu, struct vm* vm) { }

void vcpu_arch_reset(struct vcpu* vcpu, vaddr_t entry) { }

bool vcpu_arch_is_on(struct vcpu* vcpu)
{
    return true;
}

unsigned long vcpu_readreg(struct vcpu* vcpu, unsigned long reg)
{
    return 0;
}

void vcpu_writereg(struct vcpu* vcpu, unsigned long reg, unsigned long val) { }

void vcpu_restore_state(struct vcpu* vcpu) { }

void vcpu_save_state(struct vcpu* vcpu) { }

void vcpu_arch_entry() { }
