/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <bao.h>

#include <cpu.h>
#include <mem.h>
#include <arch/plat.h>
#include <interrupts.h>
#include <console.h>
#include <printk.h>
#include <platform.h>
#include <vmm.h>
#include <drivers/renesas_rlin3.h>

unsigned int boot_ctrl = 0;

struct renesas_rlin3* uart = (struct renesas_rlin3*)(0xFFC7C100);

void init(cpuid_t cpu_id)
{
    if (cpu_is_master()) {
        // uart_init(uart);
        // uart_enable(uart);
    }

    /**
     * These initializations must be executed first and in fixed order.
     */

    // console_printk("init_1\n\r");
    cpu_init(cpu_id);
    mem_init();

    plat_init();

    /* -------------------------------------------------------------- */

    console_init();

    if (cpu_is_master()) {
        console_printk("Bao Hypervisor\n\r");
        console_printk("BOOTCTRL = %x\n\r", boot_ctrl);
    }
    
    interrupts_init();

    vmm_init();

    /* Should never reach here */
    while (1) { }
}
