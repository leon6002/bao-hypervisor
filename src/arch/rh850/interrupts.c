/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include "inc/arch/intc.h"
#include <bao.h>
#include <interrupts.h>

#include <cpu.h>
#include <mem.h>
#include <platform.h>
#include <vm.h>
#include <fences.h>
#include <arch/sysregs.h>

extern irq_handler_t interrupt_handlers[MAX_INTERRUPT_HANDLERS];

void interrupts_arch_init()
{
    intc_init();
}

void interrupts_arch_enable(irqid_t int_id, bool en)
{
    intc_set_enable(int_id, en);
    intc_set_prio(int_id, 0x01);
    intc_set_trgt(int_id, cpu()->id);
}

void interrupts_arch_handle(void)
{
    ERROR("not implemented");
}

bool interrupts_arch_check(irqid_t int_id)
{
    return intc_get_pend(int_id);
}

void interrupts_arch_clear(irqid_t int_id)
{
    intc_set_pend(int_id, false);
}

irqid_t interrupts_arch_reserve(irqid_t int_id)
{
    intc_hyp_assign(int_id);

    return int_id;
}

inline bool interrupts_arch_conflict(bitmap_t* interrupt_bitmap, irqid_t int_id)
{
    return bitmap_get(interrupt_bitmap, int_id);
}

void interrupts_arch_vm_assign(struct vm* vm, irqid_t int_id)
{
    intc_vm_assign(int_id, vm->id);
}

void interrupts_arch_ipi_send(cpuid_t cpu_target, irqid_t ipi_id)
{
    UNUSED_ARG(cpu_target);
    UNUSED_ARG(ipi_id);
    ERROR("not implemented");
}
