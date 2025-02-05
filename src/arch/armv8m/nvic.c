/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <arch/nvic.h>
#include "arch/interrupts.h"
#include <interrupts.h>
#include <cpu.h>
#include <fences.h>
#include <platform_defs.h>
#include <platform.h>

#include <vm.h>

BITMAP_ALLOC(valid, NVIC_MAX_INTERRUPTS);

void nvic_init_ipi(void) { }

void nvic_init(void) { }

void nvic_cpu_init(void)
{
    /* Nothing to do */
}

void nvic_set_enbl(struct nvic* ic, irqid_t int_id, bool en)
{
    if (en) {
        ic->iser[((int_id) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)int_id) & 0x1FUL));
    } else {
        ic->icer[((int_id) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)int_id) & 0x1FUL));
    }
    DSB();
    ISB();
}

bool nvic_get_enbl(struct nvic* ic, irqid_t int_id)
{
    return ((uint32_t)(((ic->iser[(((uint32_t)int_id) >> 5UL)] &
                            (1UL << (((uint32_t)int_id) & 0x1FUL))) != 0UL) ?
            true :
            false));
}

void nvic_set_prio(struct nvic* ic, irqid_t int_id, uint32_t prio)
{
    ic->ipr[((uint32_t)int_id)] = (uint8_t)((prio << (8U - NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
}

uint32_t nvic_get_prio(struct nvic* ic, irqid_t int_id)
{
    return ((uint32_t)ic->ipr[((uint32_t)int_id)] >> (8U - NVIC_PRIO_BITS));
}

bool nvic_get_pend(struct nvic* ic, irqid_t int_id)
{
    return ((uint32_t)(((ic->ispr[(((uint32_t)int_id) >> 5UL)] &
                            (1UL << (((uint32_t)int_id) & 0x1FUL))) != 0UL) ?
            true :
            false));
}

void nvic_set_pend(struct nvic* ic, irqid_t int_id)
{
    ic->ispr[(((uint32_t)int_id) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)int_id) & 0x1FUL));
}

void nvic_clr_pend(struct nvic* ic, irqid_t int_id)
{
    ic->icpr[(((uint32_t)int_id) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)int_id) & 0x1FUL));
}

bool nvic_get_target(irqid_t int_id)
{
    return ((uint32_t)(((NVIC->itns[(((uint32_t)int_id) >> 5UL)] &
                            (1UL << (((uint32_t)int_id) & 0x1FUL))) != 0UL) ?
            true :
            false));
}

void nvic_set_target(irqid_t int_id)
{
    // Sets to NS
    NVIC->itns[(((uint32_t)int_id) >> 5UL)] |= ((uint32_t)(1UL << (((uint32_t)int_id) & 0x1FUL)));
}

void nvic_clear_target(irqid_t int_id)
{
    // Clears to S
    NVIC->itns[(((uint32_t)int_id) >> 5UL)] &= ~((uint32_t)(1UL << (((uint32_t)int_id) & 0x1FUL)));
}

bool nvic_get_active(struct nvic* ic, irqid_t int_id)
{
    return ((uint32_t)(((ic->iabr[(((uint32_t)int_id) >> 5UL)] &
                            (1UL << (((uint32_t)int_id) & 0x1FUL))) != 0UL) ?
            true :
            false));
}

void nvic_handle(void) { }

void nvic_send_ipi(cpuid_t target_cpu)
{
    UNUSED_ARG(target_cpu);
}

void nvic_config_irq(irqid_t int_id, bool en)
{
    UNUSED_ARG(int_id);
    UNUSED_ARG(en);
}

void nvic_assign_int_to_vm(struct vm* vm, irqid_t id)
{
    UNUSED_ARG(vm);
    UNUSED_ARG(id);
}

void nvic_assign_icu_to_vm(unsigned long id, struct vm* vm)
{
    UNUSED_ARG(id);
    UNUSED_ARG(vm);
}

bool nvic_id_valid(unsigned long id)
{
    UNUSED_ARG(id);
    return false;
}
