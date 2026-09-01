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
                 * MPU budget: the per-core MPU has 32 entries and Bao's own
                 * mappings take roughly a quarter of them; mpu_add_region()
                 * fails silently once the bitmap is full, and every dev past
                 * the cutoff faults as "no emulation handler" even though it
                 * is declared here. Adjacent pages are therefore merged into
                 * wide windows: 4 regions + 16 devs = 20 guest entries.
                 * Over-granting inside a window is harmless in a single-VM
                 * setup -- the guest owns the whole chip anyway.
                 */
                .dev_num = 19,
                .devs = (struct vm_dev_region[]){
                    /* Local RAM, self view (each core sees its own) */
                    { .pa = 0xFDE00000, .va = 0xFDE00000, .size = 0x10000,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* Local RAM global views (per-core stacks of PE1..PE3
                     * are linked at these addresses) */
                    { .pa = 0xFDC00000, .va = 0xFDC00000, .size = 0x10000,
                      .interrupt_num = 0, .interrupts = NULL },
                    { .pa = 0xFDA00000, .va = 0xFDA00000, .size = 0x10000,
                      .interrupt_num = 0, .interrupts = NULL },
                    { .pa = 0xFD800000, .va = 0xFD800000, .size = 0x10000,
                      .interrupt_num = 0, .interrupts = NULL },
                    { .pa = 0xFD600000, .va = 0xFD600000, .size = 0x10000,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* SYSCTRL cluster, one window: module standby + key
                     * protection (+0x1000), flash high-voltage enables FHVE
                     * (+0x4800, written by RFD), AWO standby (+0x8E00) */
                    { .pa = 0xFF980000, .va = 0xFF980000, .size = 0xF000,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* Timer/watchdog block, one window: OSTM0..3 ticks
                     * (+0x0000, IRQs 199..202), WDTB0..3 (+0x1000, INTC1
                     * ch22 -- the 75% trigger interrupt, one per PE, which
                     * is the only thing that ever feeds the dog: the driver
                     * kicks it once in Wdg_Init and reloads it from its own
                     * ISR thereafter), port noise filter add-page (+0x6900),
                     * TAUJ0 1ms GPT (+0x7000, IRQ 360) */
                    { .pa = 0xFFBF0000, .va = 0xFFBF0000, .size = 0x8000,
                      .interrupt_num = 6,
                      .interrupts = (irqid_t[]){ 22, 199, 200, 201, 202, 360 } },
                    /* RSCFD0: channel 2 (board connector CAN1) + globals */
                    { .pa = 0xFFF50000, .va = 0xFFF50000, .size = 0x20000,
                      .interrupt_num = 5,
                      .interrupts = (irqid_t[]){ 296, 297, 304, 305, 306 } },
                    /* IPIR is deliberately NOT mapped: Bao emulates the whole
                     * page (vipir.c) -- it forwards channels 0..2, protects
                     * its own channel 3 and assigns the channel interrupts to
                     * the VM. A direct map would bypass all of that. */
                    /* CPU peripheral windows: the self view at +0x0 plus
                     * the four cross-PE views at 0xFFFC4000 + n*0x4000 (the
                     * Wdg driver masks its INTC1 channel via IMR0 there) */
                    { .pa = 0xFFFC0000, .va = 0xFFFC0000, .size = 0x14000,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* FEINC_PE0..PE3: the OS masks FEINT at init */
                    { .pa = 0xFF9A3B00, .va = 0xFF9A3B00, .size = 0x400,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* Flash control window (RFD/FACI, polled): FACI0 regs
                     * at +0x10000, SPID regs at +0x8000, aux at +0x40000 */
                    { .pa = 0xFFA00000, .va = 0xFFA00000, .size = 0x50000,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* Data flash ECC controller (ECCDF), probed by
                     * Fls_PreFcuInitCheck during Fls_Init */
                    { .pa = 0xFFC62C00, .va = 0xFFC62C00, .size = 0x100,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* Data flash blank-check alias area: reads report
                     * whether the corresponding DF word is blank, used by
                     * Fls_PerformBlankCheckForReadOp on every NvM read */
                    { .pa = 0xFF400000, .va = 0xFF400000, .size = 0x200000,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* Data flash, one window: user areas DFDA0..2, the
                     * property/config areas and the RFD extended area */
                    { .pa = 0xFF200000, .va = 0xFF200000, .size = 0x180000,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* Reset/standby controller block around BOOTCTRL:
                     * the MCU driver reads reset and standby status here.
                     * BOOTCTRL itself (0xFFFB2000) stays emulated -- Bao
                     * installs its handler over this mapping. */
                    { .pa = 0xFFFB2800, .va = 0xFFFB2800, .size = 0x800,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* RAM ECC capture block (MECCCAP_*): the guest's MCU
                     * driver arms the local- and cluster-RAM ECC error
                     * interrupts here as part of Mcu_Init */
                    { .pa = 0xFFC50000, .va = 0xFFC50000, .size = 0x9000,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* ECM and the diagnostic block around it, one window:
                     * the error thresholds that go with the full-speed
                     * clock live here (0xFFD38000 page included) */
                    { .pa = 0xFFD30000, .va = 0xFFD30000, .size = 0xB000,
                      .interrupt_num = 0, .interrupts = NULL },
                    /* Port block, ECM page, noise filters (kept while the
                     * guest runs its own Port_Init) */
                    { .pa = 0xFFD90000, .va = 0xFFD90000, .size = 0x7000,
                      .interrupt_num = 0, .interrupts = NULL },
                    { .pa = 0xFFED0000, .va = 0xFFED0000, .size = 0x8000,
                      .interrupt_num = 0, .interrupts = NULL },
                },
            },
        },
    },
};
