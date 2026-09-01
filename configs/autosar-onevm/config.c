/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

/*
 * The configuration that passed acceptance on 2026-09-01: the guest reproduced
 * its bare-metal CAN signature with every period inside 1 ms and no resets,
 * paired with guest 35819cd.
 *
 * It maps only what the demo turned out to need, which is how it was built --
 * one unmapped block at a time, over about seventy board rounds. Kept under its
 * own name so there is always something known-good to build and compare
 * against; autosar-onevm is the one that gets developed.
 */

#include <config.h>

/* Every interrupt this part has, assigned to the one VM.
 *
 * With a single VM there is nothing to partition, so naming interrupts one at a
 * time only creates a way to forget one -- and forgetting one is silent. An
 * interrupt the VM was not given is dropped by the emulated INTC without a
 * word, which is how the watchdog trigger went missing: INTC1 channel 22 was
 * absent from the config, the guest's attempt to route it disappeared, the dog
 * was never fed, and the board reset every 0.6 s while looking like slow
 * timing.
 *
 * The list is generated, not maintained: scripts/gen_irqs.py reads the
 * manual's own interrupt table. It drops the channels marked Reserved and the
 * ones column P says this package does not have, because intc_vm_assign writes
 * EIBD for every id it is handed and a write that will not stick ends in
 * ERROR(). It drops channel 3 because that is the hypervisor's own IPI.
 *
 * ⚠ Assign each id exactly once. A second assignment of the same id is a
 * conflict, and vm_init_dev turns a refused assignment into a fatal error, so
 * the per-device lists this replaces had to go rather than stay alongside. */
static irqid_t single_vm_irqs[] = {0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 34, 36, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382, 383, 384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399, 400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 542, 543, 544, 545, 546, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 577, 578, 579, 580, 581, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597, 598, 599, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611, 612, 613, 614, 615, 616, 617, 618, 619, 620, 621, 622, 623, 624, 625, 626, 627, 628, 629, 630, 631, 632, 633, 634, 635, 636, 637, 638, 639, 640, 641, 642, 643, 644, 645, 646, 647, 648, 649, 650, 651, 652, 653, 654, 655, 656, 657, 658, 659, 660, 661, 662, 663, 664, 665, 666, 667, 668, 669, 670, 671, 672, 673, 674, 675, 676, 677, 678, 679, 680, 681, 682, 683, 684, 685, 686, 687, 688, 689, 690, 691, 692, 693, 694, 696, 697, 698, 699, 700, 701, 702, 703, 704, 705, 706, 715, 716,};
#define SINGLE_VM_IRQ_NUM (sizeof(single_vm_irqs) / sizeof(single_vm_irqs[0]))


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
                      .interrupt_num = SINGLE_VM_IRQ_NUM, .interrupts = single_vm_irqs },
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
                      .interrupt_num = 0, .interrupts = NULL },
                    /* RSCFD0: channel 2 (board connector CAN1) + globals */
                    { .pa = 0xFFF50000, .va = 0xFFF50000, .size = 0x20000,
                      .interrupt_num = 0, .interrupts = NULL },
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
