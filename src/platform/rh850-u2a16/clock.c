/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <platform.h>

volatile struct clk_ctl_iso_hw* clk_ctl_iso;
volatile struct clk_ctl_awo_hw* clk_ctl_awo;

void clock_init(void)
{
    size_t stab_cnt = 0;

    /* Map ISO clock controller MMIO */
    vaddr_t clk_iso_ptr = mem_alloc_map_dev(&cpu()->as, SEC_HYP_PRIVATE, INVALID_VA,
        (paddr_t)(PLAT_CLK_ISO_BASE), NUM_PAGES(sizeof(struct clk_ctl_iso_hw)));
    if (clk_iso_ptr == INVALID_VA) {
        ERROR("Maping clock ISO area failed\n");
    }
    clk_ctl_iso = (struct clk_ctl_iso_hw*)clk_iso_ptr;

    /* Map AWO clock controller MMIO */
    vaddr_t clk_awo_ptr = mem_alloc_map_dev(&cpu()->as, SEC_HYP_PRIVATE, INVALID_VA,
        (paddr_t)(PLAT_CLK_AWO_BASE), NUM_PAGES(0x600));
    if (clk_awo_ptr == INVALID_VA) {
        ERROR("maping clock AWO area failed\n");
    }
    clk_ctl_awo = (struct clk_ctl_awo_hw*)clk_awo_ptr;

    /* Disable CLK registers protection */
    clk_ctl_iso->CLKKCPROT1 = CLKKCPROT1_ENABLE_WR;

    /* Start main oscillator, if not started */
    if (!(clk_ctl_awo->MOSCS & MOSCS_MOSCEN_BIT) && !(clk_ctl_awo->MOSCS & MOSCS_MOSCSTAB_BIT)) {
        /* Start the Main OSC */
        clk_ctl_awo->MOSCE = MOSCE_MOSCENTRG_BIT;
        /* Confirm that the Main OSC has been stable */
        while (!(clk_ctl_awo->MOSCS & MOSCS_MOSCEN_BIT) ||
            !(clk_ctl_awo->MOSCS & MOSCS_MOSCSTAB_BIT))
            ;
        /* Wait 1.4us or Read the MOSCS.MOSCSTAB = 1 four times */
        while (stab_cnt < 4) {
            stab_cnt += ((clk_ctl_awo->MOSCS & MOSCS_MOSCSTAB_BIT) >> 1);
        }
    }

    /*
     * The PLL and the CPU clock selector are deliberately left alone.
     *
     * The guest's MCU driver configures the PLL only when it finds it
     * stopped; starting it here makes that driver skip its own setup, so
     * the PLL keeps a default multiplier and every derived clock -- the
     * peripheral bus feeding CAN included -- comes out at half the
     * frequency the guest's bit timing assumes. That is what put the
     * AUTOSAR guest on the wire at 250 kbit/s instead of 500. The
     * reference image that first ran an AUTOSAR guest here touches only
     * the main oscillator and module standby for the same reason.
     *
     * Bao itself runs fine on the reset-default clock; the guest owns the
     * PLL and the clock tree that hangs off it.
     */

    /* Set CLKs to continue in standby mode */
    clk_ctl_iso->PLLSTPM = 1UL;
    clk_ctl_awo->MOSCSTPM = 1UL;

    /* Enable CLK registers protection */
    clk_ctl_iso->CLKKCPROT1 = CLKKCPROT1_DISABLE_WR;

    mem_unmap(&cpu()->as, (vaddr_t)(PLAT_CLK_ISO_BASE), NUM_PAGES(sizeof(struct clk_ctl_iso_hw)),
        true);
    mem_unmap(&cpu()->as, (vaddr_t)(PLAT_CLK_AWO_BASE), NUM_PAGES(sizeof(struct clk_ctl_awo_hw)),
        true);
}
