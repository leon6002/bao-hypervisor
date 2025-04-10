/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <arch/plat.h>

/* 
    Array of port register values

    Index:  |0 |1 |2 |3 |4 |5  |6  |7  |8  |9  |10 |
    Groups: |G2|G3|G4|G5|G6|G10|G17|G20|G21|G22|G24|
*/

uint16_t port_reg_val[PLAT_NUM_PORT_REGS][PLAT_NUM_PORT_GROUPS] = {
    /* PMC */     { 0x7830U, 0x003CU, 0x1BC0U, 0x0004U, 0x0B40U, 0x3F18U, 0x0000U, 0x66C8U, 0x00E0U, 0x001FU, 0x0000U },
    /* PM */      { 0x8CD3U, 0xFFEBU, 0x3A5FU, 0xFFEBU, 0xFE2FU, 0xF39CU, 0xFFDCU, 0xB9A7U, 0xFF1FU, 0xFFE2U, 0xFFFFU },
    /* PIPC */    { 0x2000U, 0x0000U, 0x0000U, 0x0004U, 0x0000U, 0x0400U, 0x0000U, 0x0008U, 0x00E0U, 0x0001U, 0x0000U },
    /* PIBC */    { 0x0001U, 0x0040U, 0x2003U, 0x0040U, 0x1020U, 0x0000U, 0x0000U, 0x0100U, 0x0000U, 0x0000U, 0x0000U },
    /* PFC */     { 0x0000U, 0x0034U, 0x0340U, 0x0000U, 0x0340U, 0x2118U, 0x0000U, 0x66C8U, 0x00E0U, 0x001FU, 0x0000U },
    /* PFCE */    { 0x7830U, 0x0008U, 0x0000U, 0x0004U, 0x0B40U, 0x1E00U, 0x0000U, 0x0000U, 0x0000U, 0x0013U, 0x0000U },
    /* PFCAE */   { 0x0010U, 0x0008U, 0x18C0U, 0x0000U, 0x0000U, 0x2100U, 0x0000U, 0x0000U, 0x00E0U, 0x000CU, 0x0000U },
    /* PU */      { 0x0000U, 0x0000U, 0x0200U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* PD */      { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* PIS */     { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* PISA */    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0018U, 0x0000U, 0x6680U, 0x0000U, 0x0000U, 0x0000U },
    /* PODC */    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0008U, 0x0000U, 0x0000U, 0x0000U },
    /* PODCE */   { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* PDSC */    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0018U, 0x0000U, 0x6680U, 0x0000U, 0x0000U, 0x0000U },
    /* PUCC */    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* PINV */    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* PBDC */    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* P */       { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0800U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* PSFTSE */  { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* PSFTS */   { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U },
    /* PSFC */    { 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U }
};

/* Array of port register offsets */
uint32_t port_reg_offset[PLAT_NUM_PORT_REGS] = {
    0x0014U, /* PMC */
    0x0010U, /* PM */
    0x4008U, /* PIPC */
    0x4000U, /* PIBC */
    0x0018U, /* PFC */
    0x001CU, /* PFCE */
    0x0028U, /* PFCAE */
    0x400CU, /* PU */
    0x4010U, /* PD */
    0x401CU, /* PIS */
    0x4024U, /* PISA */
    0x4014U, /* PODC */
    0x4038U, /* PODCE */
    0x4018U, /* PDSC */
    0x4028U, /* PUCC */
    0x0030U, /* PINV */
    0x4004U, /* PBDC */
    0x0000U, /* P */
    0x6014U, /* PSFTSE */
    0x6010U, /* PSFTS */
    0x6000U, /* PSFC */
};

/* Supported group indexes */
uint32_t group_number[PLAT_NUM_PORT_GROUPS] = {
    2,
    3,
    4,
    5,
    6,
    10,
    17,
    20,
    21,
    22,
    24
};

void plat_init(void) {

    if (cpu_is_master()) {

        // map clock controller MMIO
        vaddr_t clk_iso_ptr = mem_alloc_map_dev(&cpu()->as, SEC_HYP_PRIVATE, INVALID_VA,
                                (paddr_t)(MCU_PLLE), NUM_PAGES(0x900UL));
        if (clk_iso_ptr == INVALID_VA) {
            ERROR("maping clock ISO area failed");
        }
        vaddr_t clk_awo_ptr = mem_alloc_map_dev(&cpu()->as, SEC_HYP_PRIVATE, INVALID_VA,
                                (paddr_t)(MCU_MOSCE), NUM_PAGES(0x400UL));
        if (clk_awo_ptr == INVALID_VA) {
            ERROR("maping clock AWO area failed");
        }

        // // Start main oscillator
        // (*(volatile uint32_t*)MCU_CLKKCPROT1) = MCU_CKSC_DISABLE_REG_PROTECT_VALUE;
        // (*(volatile uint32_t*)MCU_MOSCE) = MCU_MOSCE_ENABLE_TRIGGER;
        // (*(volatile uint32_t*)MCU_CLKKCPROT1) = MCU_CKSC_ENABLE_REG_PROTECT_VALUE;

        // // Trigger start of PLL
        // (*(volatile uint32_t*)MCU_CLKKCPROT1) = MCU_CKSC_DISABLE_REG_PROTECT_VALUE;
        // (*(volatile uint32_t*)MCU_PLLE) = MCU_PLLE_ENABLE_TRIGGER;
        // (*(volatile uint32_t*)MCU_CLKKCPROT1) = MCU_CKSC_ENABLE_REG_PROTECT_VALUE;

        // Select CLK_PLLO clock source
        (*(volatile uint32_t*)MCU_CLKKCPROT1) = MCU_CKSC_DISABLE_REG_PROTECT_VALUE;
        (*(volatile uint32_t*)MCU_CKSC_CPUC) = 0;
        (*(volatile uint32_t*)MCU_CLKKCPROT1) = MCU_CKSC_ENABLE_REG_PROTECT_VALUE;

        // Set CLKs to continue in standby mode
        (*(volatile uint32_t*)MCU_CLKKCPROT1) = MCU_CKSC_DISABLE_REG_PROTECT_VALUE;
        (*(volatile uint32_t*)MCU_PLLSTPM) = 1UL;
        (*(volatile uint32_t*)MCU_MOSCSTPM) = 1UL;
        (*(volatile uint32_t*)MCU_CLKKCPROT1) = MCU_CKSC_ENABLE_REG_PROTECT_VALUE;

        mem_unmap(&cpu()->as, (vaddr_t)(MCU_PLLE), NUM_PAGES(0x900UL), true);
        mem_unmap(&cpu()->as, (vaddr_t)(MCU_MOSCE), NUM_PAGES(0x400UL), true);

        // map port MMIO (PLAT_PORT_BASE)
        vaddr_t port_ptr = mem_alloc_map_dev(&cpu()->as, SEC_HYP_PRIVATE, INVALID_VA,
                                (paddr_t)(PLAT_PORT_BASE), NUM_PAGES(0x8000UL));
        if (port_ptr == INVALID_VA) {
            ERROR("maping port area failed");
        }

        // Enable Write Port
        (*(volatile uint32_t*)MCU_PKCPROT) = PORT_PWE_ENABLE_WRITE;
        (*(volatile uint32_t*)MCU_PWE) = PORT_PWE_ALL_MASK;
        (*(volatile uint32_t*)MCU_PKCPROT) = PORT_PWE_DISABLE_WRITE;

        // Configure port registers
        for (int i = 0; i < PLAT_NUM_PORT_REGS; i++) {
            for (int j = 0; j < PLAT_NUM_PORT_GROUPS; j++) {
                uint16_t *port_reg_addr = (
                        void*)(PLAT_PORT_BASE + (0x40 * group_number[j]) + port_reg_offset[i]);
                *port_reg_addr = port_reg_val[i][j];
            }
        }

        // Disable Write Port
        (*(volatile uint32_t*)MCU_PKCPROT) = PORT_PWE_ENABLE_WRITE;
        (*(volatile uint32_t*)MCU_PWE) = 0;
        (*(volatile uint32_t*)MCU_PKCPROT) = PORT_PWE_DISABLE_WRITE;

        mem_unmap(&cpu()->as, (vaddr_t)(PLAT_PORT_BASE), NUM_PAGES(0x8000UL), true);
    }

    cpu_sync_and_clear_msgs(&cpu_glb_sync);
}