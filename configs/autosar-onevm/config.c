/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

/*
 * Single-VM configuration for a full 4-core AUTOSAR image on RH850/U2A16.
 *
 * The guest is the EasyXMen U2A16 demo (4 cores, one OS image) built with
 * HV_MODE and linked at 0x10000: the whole multi-core project runs as ONE
 * VM with vCPUs mapped 1:1 to PE0..PE3, so cross-core mechanisms (IPIR,
 * shared-RAM sync flags, spinlocks, cross-core ActivateTask) stay untouched.
 *
 * Every address below was surveyed from the guest's linker scripts and MCAL
 * configuration and cross-checked against the U2A manual via the annotated
 * device catalog in configs/baremetal-devcat. Counts are load-bearing:
 * region_num/dev_num/interrupt_num must equal the array lengths.
 */

#include <config.h>

struct config config = {

    /*
     * Bao's .data/.bss default to the first RWX region (Cluster0 at
     * 0xFE000000), which is exactly the guest's main RAM. Relocate to
     * Cluster1, the one RAM block this guest never touches.
     */
    .hyp = {
        .data_relocate = true,
        .data_addr = 0xFE100000,
    },

    .vmlist_size = 1,
    .vmlist = (struct vm_config[]){
        {
            .entry = 0x10000,
            /* Code window = relocated iROM_0 + MCAL database (dBaseROM at
             * 0x400000..0x410000); flashed separately from the GHS hex. */
            .image = VM_IMAGE_LOADED(0x10000, 0x10000, 0x400000),
            .cpu_affinity = 0xF, /* PE0..PE3 */

            .platform = {
                .cpu_num = 4,

                .region_num = 4,
                .regions = (struct vm_mem_region[]){
                    /* Guest code window (see .image) */
                    { .base = 0x00010000, .size = 0x400000 },
                    /* Cluster0: .data/.bss, PE0 stack, OS core stacks,
                     * multicore sync flags */
                    { .base = 0xFE000000, .size = 0x80000 },
                    /* Cluster2 low 512K: ETH port RAM, WdgM RAM at
                     * +0x5FC00, calibration RAM at +0x60000 */
                    { .base = 0xFE400000, .size = 0x80000 },
                    /* Retention RAM: .rcode/.rdata/.rbss/.rstack */
                    { .base = 0xFE800000, .size = 0x20000 },
                },

                /*
                 * MPU budget: 4 regions + 24 devs = 28 of the 32 per-core
                 * entries. If it ever gets tight, the four Port windows at
                 * the end drop out when the guest's Port_Init is disabled.
                 */
                .dev_num = 24,
                .devs = (struct vm_dev_region[]){
                    /* Local RAM, self view (each core sees its own) */
                    { .pa = 0xFDE00000, .va = 0xFDE00000, .size = 0x10000,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* Local RAM global views: per-core stacks of PE1..PE3
                     * are linked at these addresses (PE0's global view is
                     * used by the vendor scrub only, kept for symmetry) */
                    { .pa = 0xFDC00000, .va = 0xFDC00000, .size = 0x10000,
                      .interrupt_num = 0, .interrupts = NULL },
                    { .pa = 0xFDA00000, .va = 0xFDA00000, .size = 0x10000,
                      .interrupt_num = 0, .interrupts = NULL },
                    { .pa = 0xFD800000, .va = 0xFD800000, .size = 0x10000,
                      .interrupt_num = 0, .interrupts = NULL },
                    { .pa = 0xFD600000, .va = 0xFD600000, .size = 0x10000,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* SYSCTRL module-standby page: MSR_OSTM/MSR_RSCFD plus
                     * MSRKCPROT at +0x710, all written via R_STBC_CfgMsr */
                    { .pa = 0xFF981000, .va = 0xFF981000, .size = 0x1000,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* OSTM0..OSTM3, one OS tick per core */
                    { .pa = 0xFFBF0000, .va = 0xFFBF0000, .size = 0x40,
                      .interrupt_num = 1, .interrupts = (irqid_t[]){ 199 } },
                    { .pa = 0xFFBF0100, .va = 0xFFBF0100, .size = 0x40,
                      .interrupt_num = 1, .interrupts = (irqid_t[]){ 200 } },
                    { .pa = 0xFFBF0200, .va = 0xFFBF0200, .size = 0x40,
                      .interrupt_num = 1, .interrupts = (irqid_t[]){ 201 } },
                    { .pa = 0xFFBF0300, .va = 0xFFBF0300, .size = 0x40,
                      .interrupt_num = 1, .interrupts = (irqid_t[]){ 202 } },
                    /* RSCFD0: guest drives channel 2 (comes out on the
                     * board connector silk-screened CAN1). Interrupts:
                     * 296/297 global error + global RX FIFO, 304/305/306
                     * channel-2 error/receive/transmit. */
                    { .pa = 0xFFF50000, .va = 0xFFF50000, .size = 0x20000,
                      .interrupt_num = 5,
                      .interrupts = (irqid_t[]){ 296, 297, 304, 305, 306 } },
                    /* TAUJ0: 1 ms GPT channel 0, INTTAUJ0I0 = 360 */
                    { .pa = 0xFFBF7000, .va = 0xFFBF7000, .size = 0x100,
                      .interrupt_num = 1, .interrupts = (irqid_t[]){ 360 } },
                    /* WDTB0 watchdog. Its trigger interrupt is INTC1
                     * channel 22 (per-core); how INTC1-range channels are
                     * declared here is still open with the Bao team, so it
                     * is not listed yet. */
                    { .pa = 0xFFBF1000, .va = 0xFFBF1000, .size = 0x20,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* IPIR inter-processor interrupts (OS RemoteCall).
                     * IPIR0 is INTC1 channel 0 on every core -- same open
                     * question as WDTB0 above. */
                    { .pa = 0xFFFB9000, .va = 0xFFFB9000, .size = 0x1000,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* CPU peripheral self window: INTC1 EIC writes for
                     * channels < 32 go here. Without it INTC2 interrupts
                     * look healthy while INTC1 ones never fire. */
                    { .pa = 0xFFFC0000, .va = 0xFFFC0000, .size = 0x4000,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* FEINC_PE0..PE3: the OS masks FEINT at init */
                    { .pa = 0xFF9A3B00, .va = 0xFF9A3B00, .size = 0x400,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* Flash control registers (RFD, polled -- no IRQs) */
                    { .pa = 0xFFA08000, .va = 0xFFA08000, .size = 0x1000,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* Data flash user areas DFDA0..2 (Fee/NvM) */
                    { .pa = 0xFF200000, .va = 0xFF200000, .size = 0x50000,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* Data flash property/config areas read by RFD */
                    { .pa = 0xFF320000, .va = 0xFF320000, .size = 0x21000,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* Data flash extended area read by RFD */
                    { .pa = 0xFF374000, .va = 0xFF374000, .size = 0x1000,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* Port block, ECM page and noise filters: needed only
                     * while the guest keeps its own Port_Init (Bao's
                     * platform layer does not set up this guest's CAN pin
                     * muxing, so it does for now). */
                    { .pa = 0xFFD90000, .va = 0xFFD90000, .size = 0x7000,
                      .interrupt_num = 0, .interrupts = NULL },
                    { .pa = 0xFFD38000, .va = 0xFFD38000, .size = 0x1000,
                      .interrupt_num = 0, .interrupts = NULL },
                    { .pa = 0xFFED0000, .va = 0xFFED0000, .size = 0x8000,
                      .interrupt_num = 0, .interrupts = NULL },
                    { .pa = 0xFFBF6900, .va = 0xFFBF6900, .size = 0x100,
                      .interrupt_num = 0, .interrupts = NULL },
                },
            },
        },
    },
};
