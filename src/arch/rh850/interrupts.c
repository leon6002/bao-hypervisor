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

#define IPIR_CHANNEL_NUM 4
#define IPI_IRQ_ID       0

irqid_t interrupts_ipi_id = IPI_IRQ_ID;

struct ipir {
    struct {
        volatile uint8_t IPI_ENS;   // Offset: 0x000 + 0x020 * n
        volatile uint8_t IPI_FLGS;  // Offset: 0x004 + 0x020 * n
        volatile uint8_t IPI_FCLRS; // Offset: 0x008 + 0x020 * n
        volatile uint8_t IPI_REQS;  // Offset: 0x010 + 0x020 * n
        volatile uint8_t IPI_RCLRS; // Offset: 0x014 + 0x020 * n
    } channel[IPIR_CHANNEL_NUM];
};

struct ipir* ipir_hw;

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

void interrupts_arch_ipi_send(cpuid_t cpu_target)
{
    ipir_hw->channel[0].IPI_REQS = (1 << cpu_target);
}

static void ipir_map_global_mmio()
{
    vaddr_t start_addr = platform.arch.ipir_addr;
    size_t npages = NUM_PAGES(sizeof(struct ipir));

    ipir_hw = mem_alloc_map_dev(&cpu()->as, SEC_HYP_GLOBAL, INVALID_VA, start_addr, npages);
    if (ipir_hw == INVALID_VA) {
        ERROR("maping ipir failed");
    }
}

enum irq_res ipir_handle(irqid_t int_id)
{
    cpu_msg_handler();

    /* ipi clear */
    cpuid_t from = ipir_hw->channel[IPI_IRQ_ID].IPI_FLGS;
    ipir_hw->channel[IPI_IRQ_ID].IPI_FCLRS = ((1 << from) - 1);
}

void interrupts_arch_ipi_init(void)
{
    if (cpu_is_master()) {
        ipir_map_global_mmio();
    }
    /* TODO How to deal with interrupt being "shared" between cores */
    interrupts_reserve(IPI_IRQ_ID, ipir_handle);

    ipir_hw->channel[IPI_IRQ_ID].IPI_ENS = ((1 << PLAT_CPU_NUM) - 1);
}

void interrupts_arch_ipi_enable(void)
{
    interrupts_cpu_enable(IPI_IRQ_ID, true);
}
