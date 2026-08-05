/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

/*
 * RH850/U2A-EVA device catalogue -- one baremetal VM listing the peripherals a guest is
 * likely to want, so a production configuration can be assembled by copying entries out of
 * here rather than looking each one up again.
 *
 * Sources, both checked entry by entry rather than assumed:
 *   - addresses and sizes: peripheral address map, RH850/U2A-EVA Group User's Manual:
 *     Hardware (R01UH0864EJ0150 Rev.1.50, 2024-08-30)
 *   - interrupt numbers: the per-peripheral interrupt tables in the same manual
 *   - cpu_num: RH850/U2A Datasheet (R01DS0494ED0100), which gives U2A-EVA and U2A16 four
 *     main cores and 32 MPU regions per core
 *
 * Each entry carries the manual's own address range and interrupt names in a comment, so any
 * line can be traced back to its source.
 *
 * This is NOT meant to be flashed as is. Handing one VM every peripheral defeats the point of
 * a hypervisor, and there are 32 MPU entries per core against far more devices listed here.
 * Take what a guest actually needs.
 *
 * Still missing, and not derivable from the address map:
 *   - clocks and pin muxing. A peripheral that is mapped but unclocked or unrouted reads back
 *     zeroes. That setup belongs to the platform layer.
 *   - module standby. Several blocks are stopped out of reset until their MSR_* register is
 *     cleared; MSR_RSCFD at 0xFF981000 is the one CAN needs on this board.
 *   - CAN interrupts are per channel, not per controller, so RSCFD entries are left empty:
 *     pick the channels in use. RSCFD0 covers CAN0..CAN7 at 298..321, with the two global
 *     interrupts at 296 and 297.
 */

#include <config.h>

struct config config = {

    .shmemlist_size = 0,
    .shmemlist = NULL,

    .vmlist_size = 1,

    .vmlist = (struct vm_config[]){
        {
            .entry = 0x10000,
            .image = VM_IMAGE_LOADED(0x10000, 0x10000, 0x30000),

            /* All four cores. The platform declares cpu_num = 4 to match the datasheet. */
            .cpu_affinity = 0xF,

            .platform = {
                .cpu_num = 4,

                .region_num = 2,
                .regions = (struct vm_mem_region[]){
                    /* Guest code, flashed separately */
                    { .base = 0x10000,    .size = 0x30000, },
                    /* Guest RAM -- Cluster RAM */
                    { .base = 0xFE000000, .size = 0x80000, },
                },

                .ipc_num = 0,
                .ipcs = NULL,
                .dev_num = 214,
                .devs = (struct vm_dev_region[]){

                    /* ---- always needed ------------------------------------------------ */

                    /* Local RAM of this core.  FDE0 0000H..FDE0 FFFFH */
                    { .pa = 0xFDE00000, .va = 0xFDE00000, .size = 0x10000,
                      .interrupt_num = 0, .interrupts = NULL, },

                    /* CPU peripheral (self), FFFC 0000H..FFFC 3FFFH -- INTC1 lives here.
                     * A guest that takes interrupts needs this; leaving it out is what makes
                     * one look healthy on the console yet never receive an interrupt. */
                    { .pa = 0xFFFC0000, .va = 0xFFFC0000, .size = 0x4000,
                      .interrupt_num = 0, .interrupts = NULL, },

                    /* ---- CAN ------------------------------------------------------- */
                    /* RSCFD1   FFD0 0000H..FFD1 FFFFH */
                    { .pa = 0xFFD00000, .va = 0xFFD00000, .size = 0x20000,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RSCFD0   FFF5 0000H..FFF6 FFFFH */
                    { .pa = 0xFFF50000, .va = 0xFFF50000, .size = 0x20000,
                      .interrupt_num = 0, .interrupts = NULL, },

                    /* ---- LIN / UART ------------------------------------------------ */
                    /* RLIN31   FFC7 C000H..FFC7 C03FH
                     *   INTRLIN31UR0               421
                     *   INTRLIN31UR1               422
                     *   INTRLIN31UR2               423
                     */
                    { .pa = 0xFFC7C000, .va = 0xFFC7C000, .size = 0x40,
                      .interrupt_num = 3,
                      .interrupts = (irqid_t[]){ 421, 422, 423 }, },
                    /* RLIN33   FFC7 C080H..FFC7 C0BFH
                     *   INTRLIN33UR0               429
                     *   INTRLIN33UR1               430
                     *   INTRLIN33UR2               431
                     */
                    { .pa = 0xFFC7C080, .va = 0xFFC7C080, .size = 0x40,
                      .interrupt_num = 3,
                      .interrupts = (irqid_t[]){ 429, 430, 431 }, },
                    /* RLIN35   FFC7 C100H..FFC7 C13FH
                     *   INTRLIN35UR0               437
                     *   INTRLIN35UR1               438
                     *   INTRLIN35UR2               439
                     */
                    { .pa = 0xFFC7C100, .va = 0xFFC7C100, .size = 0x40,
                      .interrupt_num = 3,
                      .interrupts = (irqid_t[]){ 437, 438, 439 }, },
                    /* RLIN37   FFC7 C180H..FFC7 C1BFH
                     *   INTRLIN37UR0               445
                     *   INTRLIN37UR1               446
                     *   INTRLIN37UR2               447
                     */
                    { .pa = 0xFFC7C180, .va = 0xFFC7C180, .size = 0x40,
                      .interrupt_num = 3,
                      .interrupts = (irqid_t[]){ 445, 446, 447 }, },
                    /* RLIN39   FFC7 C200H..FFC7 C23FH
                     *   INTRLIN39UR0               453
                     *   INTRLIN39UR1               454
                     *   INTRLIN39UR2               455
                     */
                    { .pa = 0xFFC7C200, .va = 0xFFC7C200, .size = 0x40,
                      .interrupt_num = 3,
                      .interrupts = (irqid_t[]){ 453, 454, 455 }, },
                    /* RLIN311   FFC7 C280H..FFC7 C2BFH */
                    { .pa = 0xFFC7C280, .va = 0xFFC7C280, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RLIN313   FFC7 C300H..FFC7 C33FH */
                    { .pa = 0xFFC7C300, .va = 0xFFC7C300, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RLIN315   FFC7 C380H..FFC7 C3BFH */
                    { .pa = 0xFFC7C380, .va = 0xFFC7C380, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RLIN317   FFC7 C400H..FFC7 C43FH */
                    { .pa = 0xFFC7C400, .va = 0xFFC7C400, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RLIN319   FFC7 C480H..FFC7 C4BFH */
                    { .pa = 0xFFC7C480, .va = 0xFFC7C480, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RLIN321   FFC7 C500H..FFC7 C53FH */
                    { .pa = 0xFFC7C500, .va = 0xFFC7C500, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RLIN323   FFC7 C580H..FFC7 C5BFH */
                    { .pa = 0xFFC7C580, .va = 0xFFC7C580, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RLIN30   FFD2 8000H..FFD2 803FH
                     *   INTRLIN30UR0               417
                     *   INTRLIN30UR1               418
                     *   INTRLIN30UR2               419
                     */
                    { .pa = 0xFFD28000, .va = 0xFFD28000, .size = 0x40,
                      .interrupt_num = 3,
                      .interrupts = (irqid_t[]){ 417, 418, 419 }, },
                    /* RLIN32   FFD2 8200H..FFD2 823FH
                     *   INTRLIN32UR0               425
                     *   INTRLIN32UR1               426
                     *   INTRLIN32UR2               427
                     */
                    { .pa = 0xFFD28200, .va = 0xFFD28200, .size = 0x40,
                      .interrupt_num = 3,
                      .interrupts = (irqid_t[]){ 425, 426, 427 }, },
                    /* RLIN34   FFD2 8400H..FFD2 843FH
                     *   INTRLIN34UR0               433
                     *   INTRLIN34UR1               434
                     *   INTRLIN34UR2               435
                     */
                    { .pa = 0xFFD28400, .va = 0xFFD28400, .size = 0x40,
                      .interrupt_num = 3,
                      .interrupts = (irqid_t[]){ 433, 434, 435 }, },
                    /* RLIN36   FFD2 8600H..FFD2 863FH
                     *   INTRLIN36UR0               441
                     *   INTRLIN36UR1               442
                     *   INTRLIN36UR2               443
                     */
                    { .pa = 0xFFD28600, .va = 0xFFD28600, .size = 0x40,
                      .interrupt_num = 3,
                      .interrupts = (irqid_t[]){ 441, 442, 443 }, },
                    /* RLIN38   FFD2 8800H..FFD2 883FH
                     *   INTRLIN38UR0               449
                     *   INTRLIN38UR1               450
                     *   INTRLIN38UR2               451
                     */
                    { .pa = 0xFFD28800, .va = 0xFFD28800, .size = 0x40,
                      .interrupt_num = 3,
                      .interrupts = (irqid_t[]){ 449, 450, 451 }, },
                    /* RLIN310   FFD2 8A00H..FFD2 8A3FH */
                    { .pa = 0xFFD28A00, .va = 0xFFD28A00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RLIN312   FFD2 8C00H..FFD2 8C3FH */
                    { .pa = 0xFFD28C00, .va = 0xFFD28C00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RLIN314   FFD2 8E00H..FFD2 8E3FH */
                    { .pa = 0xFFD28E00, .va = 0xFFD28E00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RLIN316   FFD2 9000H..FFD2 903FH */
                    { .pa = 0xFFD29000, .va = 0xFFD29000, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RLIN318   FFD2 9200H..FFD2 923FH */
                    { .pa = 0xFFD29200, .va = 0xFFD29200, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RLIN320   FFD2 9400H..FFD2 943FH */
                    { .pa = 0xFFD29400, .va = 0xFFD29400, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RLIN322   FFD2 9600H..FFD2 963FH */
                    { .pa = 0xFFD29600, .va = 0xFFD29600, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },

                    /* ---- SPI ------------------------------------------------------- */
                    /* MSPITG   FFC7 5D00H..FFC7 5D3FH */
                    { .pa = 0xFFC75D00, .va = 0xFFC75D00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* MSPI0   FFC7 6000H..FFC7 7FFFH */
                    { .pa = 0xFFC76000, .va = 0xFFC76000, .size = 0x2000,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* MSPI1   FFC7 E000H..FFC7 FFFFH */
                    { .pa = 0xFFC7E000, .va = 0xFFC7E000, .size = 0x2000,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* MSPI3   FFCC 0000H..FFCC 1FFFH */
                    { .pa = 0xFFCC0000, .va = 0xFFCC0000, .size = 0x2000,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* MSPI5   FFCC 4000H..FFCC 5FFFH */
                    { .pa = 0xFFCC4000, .va = 0xFFCC4000, .size = 0x2000,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* MSPI7   FFCC 8000H..FFCC 9FFFH */
                    { .pa = 0xFFCC8000, .va = 0xFFCC8000, .size = 0x2000,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* MSPI9   FFCC C000H..FFCC DFFFH */
                    { .pa = 0xFFCCC000, .va = 0xFFCCC000, .size = 0x2000,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* MSPI2   FFD8 0000H..FFD8 1FFFH */
                    { .pa = 0xFFD80000, .va = 0xFFD80000, .size = 0x2000,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* MSPI4   FFD8 2000H..FFD8 3FFFH */
                    { .pa = 0xFFD82000, .va = 0xFFD82000, .size = 0x2000,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* MSPI6   FFD8 4000H..FFD8 5FFFH */
                    { .pa = 0xFFD84000, .va = 0xFFD84000, .size = 0x2000,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* MSPI8   FFD8 6000H..FFD8 7FFFH */
                    { .pa = 0xFFD86000, .va = 0xFFD86000, .size = 0x2000,
                      .interrupt_num = 0, .interrupts = NULL, },

                    /* ---- SENT ------------------------------------------------------ */
                    /* RSENT0   FFD3 3600H..FFD3 367FH */
                    { .pa = 0xFFD33600, .va = 0xFFD33600, .size = 0x80,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RSENT1   FFD3 3700H..FFD3 377FH */
                    { .pa = 0xFFD33700, .va = 0xFFD33700, .size = 0x80,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RSENT2   FFD3 3800H..FFD3 387FH */
                    { .pa = 0xFFD33800, .va = 0xFFD33800, .size = 0x80,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RSENT3   FFD3 3900H..FFD3 397FH */
                    { .pa = 0xFFD33900, .va = 0xFFD33900, .size = 0x80,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RSENT4   FFD3 3A00H..FFD3 3A7FH */
                    { .pa = 0xFFD33A00, .va = 0xFFD33A00, .size = 0x80,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RSENT5   FFD3 3B00H..FFD3 3B7FH */
                    { .pa = 0xFFD33B00, .va = 0xFFD33B00, .size = 0x80,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RSENT6   FFD3 3C00H..FFD3 3C7FH */
                    { .pa = 0xFFD33C00, .va = 0xFFD33C00, .size = 0x80,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RSENT7   FFD3 3D00H..FFD3 3D7FH */
                    { .pa = 0xFFD33D00, .va = 0xFFD33D00, .size = 0x80,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* RSENTTSSEL   FFD3 3E00H..FFD3 3E03H */
                    { .pa = 0xFFD33E00, .va = 0xFFD33E00, .size = 0x4,
                      .interrupt_num = 0, .interrupts = NULL, },

                    /* ---- PSI5 ------------------------------------------------------ */
                    /* PSI5S0   FFD5 0000H..FFD5 07FFH */
                    { .pa = 0xFFD50000, .va = 0xFFD50000, .size = 0x800,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PSI5S1   FFD5 1000H..FFD5 17FFH */
                    { .pa = 0xFFD51000, .va = 0xFFD51000, .size = 0x800,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PSI50   FFD5 2000H..FFD5 21FFH */
                    { .pa = 0xFFD52000, .va = 0xFFD52000, .size = 0x200,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PSI51   FFD5 2400H..FFD5 25FFH */
                    { .pa = 0xFFD52400, .va = 0xFFD52400, .size = 0x200,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PSI52   FFD5 2800H..FFD5 29FFH */
                    { .pa = 0xFFD52800, .va = 0xFFD52800, .size = 0x200,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PSI53   FFD5 2C00H..FFD5 2DFFH */
                    { .pa = 0xFFD52C00, .va = 0xFFD52C00, .size = 0x200,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PSI5_SELB   FFD5 3000H..FFD5 3003H */
                    { .pa = 0xFFD53000, .va = 0xFFD53000, .size = 0x4,
                      .interrupt_num = 0, .interrupts = NULL, },

                    /* ---- Timers ---------------------------------------------------- */
                    /* OSTM0   FFBF 0000H..FFBF 003FH
                     *   INTOSTM0TINT               199
                     */
                    { .pa = 0xFFBF0000, .va = 0xFFBF0000, .size = 0x40,
                      .interrupt_num = 1,
                      .interrupts = (irqid_t[]){ 199 }, },
                    /* OSTM1   FFBF 0100H..FFBF 013FH
                     *   INTOSTM1TINT               200
                     */
                    { .pa = 0xFFBF0100, .va = 0xFFBF0100, .size = 0x40,
                      .interrupt_num = 1,
                      .interrupts = (irqid_t[]){ 200 }, },
                    /* OSTM2   FFBF 0200H..FFBF 023FH
                     *   INTOSTM2TINT               201
                     */
                    { .pa = 0xFFBF0200, .va = 0xFFBF0200, .size = 0x40,
                      .interrupt_num = 1,
                      .interrupts = (irqid_t[]){ 201 }, },
                    /* OSTM3   FFBF 0300H..FFBF 033FH
                     *   INTOSTM3TINT               202
                     */
                    { .pa = 0xFFBF0300, .va = 0xFFBF0300, .size = 0x40,
                      .interrupt_num = 1,
                      .interrupts = (irqid_t[]){ 202 }, },
                    /* OSTM4   FFBF 0400H..FFBF 043FH
                     *   INTOSTM4TINT               203
                     */
                    { .pa = 0xFFBF0400, .va = 0xFFBF0400, .size = 0x40,
                      .interrupt_num = 1,
                      .interrupts = (irqid_t[]){ 203 }, },
                    /* OSTM5   FFBF 0500H..FFBF 053FH
                     *   INTOSTM5TINT               204
                     */
                    { .pa = 0xFFBF0500, .va = 0xFFBF0500, .size = 0x40,
                      .interrupt_num = 1,
                      .interrupts = (irqid_t[]){ 204 }, },
                    /* OSTM6   FFBF 0600H..FFBF 063FH
                     *   INTOSTM6TINT               205
                     */
                    { .pa = 0xFFBF0600, .va = 0xFFBF0600, .size = 0x40,
                      .interrupt_num = 1,
                      .interrupts = (irqid_t[]){ 205 }, },
                    /* OSTM7   FFBF 0700H..FFBF 073FH
                     *   INTOSTM7TINT               206
                     */
                    { .pa = 0xFFBF0700, .va = 0xFFBF0700, .size = 0x40,
                      .interrupt_num = 1,
                      .interrupts = (irqid_t[]){ 206 }, },
                    /* OSTM8   FFBF 0800H..FFBF 083FH
                     *   INTOSTM8TINT               207
                     */
                    { .pa = 0xFFBF0800, .va = 0xFFBF0800, .size = 0x40,
                      .interrupt_num = 1,
                      .interrupts = (irqid_t[]){ 207 }, },
                    /* OSTM9   FFBF 0900H..FFBF 093FH
                     *   INTOSTM9TINT               208
                     */
                    { .pa = 0xFFBF0900, .va = 0xFFBF0900, .size = 0x40,
                      .interrupt_num = 1,
                      .interrupts = (irqid_t[]){ 208 }, },
                    /* ENCA0   FFBF 3000H..FFBF 307FH */
                    { .pa = 0xFFBF3000, .va = 0xFFBF3000, .size = 0x80,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* ENCA1   FFBF 3100H..FFBF 317FH */
                    { .pa = 0xFFBF3100, .va = 0xFFBF3100, .size = 0x80,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* TAUD0   FFBF 4000H..FFBF 43FFH
                     *   INTTAUD0I0                 10
                     *   INTTAUD0I1                 376
                     *   INTTAUD0I2                 11
                     *   INTTAUD0I3                 377
                     *   INTTAUD0I4                 12
                     *   INTTAUD0I5                 378
                     *   INTTAUD0I6                 13
                     *   INTTAUD0I7                 379
                     *   INTTAUD0I8                 23
                     *   INTTAUD0I9                 380
                     *   INTTAUD0I10                24
                     *   INTTAUD0I11                381
                     *   INTTAUD0I12                25
                     *   INTTAUD0I13                382
                     *   INTTAUD0I14                26
                     *   INTTAUD0I15                383
                     */
                    { .pa = 0xFFBF4000, .va = 0xFFBF4000, .size = 0x400,
                      .interrupt_num = 16,
                      .interrupts = (irqid_t[]){ 10, 376, 11, 377, 12, 378, 13, 379, 23, 380, 24, 381, 25, 382, 26, 383 }, },
                    /* TAUD1   FFBF 5000H..FFBF 53FFH
                     *   INTTAUD1I0                 384
                     *   INTTAUD1I1                 385
                     *   INTTAUD1I2                 386
                     *   INTTAUD1I3                 387
                     *   INTTAUD1I4                 388
                     *   INTTAUD1I5                 389
                     *   INTTAUD1I6                 390
                     *   INTTAUD1I7                 391
                     *   INTTAUD1I8                 392
                     *   INTTAUD1I9                 393
                     *   INTTAUD1I10                394
                     *   INTTAUD1I11                395
                     *   INTTAUD1I12                396
                     *   INTTAUD1I13                397
                     *   INTTAUD1I14                398
                     *   INTTAUD1I15                399
                     */
                    { .pa = 0xFFBF5000, .va = 0xFFBF5000, .size = 0x400,
                      .interrupt_num = 16,
                      .interrupts = (irqid_t[]){ 384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399 }, },
                    /* TAUD2   FFBF 6000H..FFBF 63FFH
                     *   INTTAUD2I0                 400
                     *   INTTAUD2I1                 401
                     *   INTTAUD2I2                 402
                     *   INTTAUD2I3                 403
                     *   INTTAUD2I4                 404
                     *   INTTAUD2I5                 405
                     *   INTTAUD2I6                 406
                     *   INTTAUD2I7                 407
                     *   INTTAUD2I8                 408
                     *   INTTAUD2I9                 409
                     *   INTTAUD2I10                410
                     *   INTTAUD2I11                411
                     *   INTTAUD2I12                412
                     *   INTTAUD2I13                413
                     *   INTTAUD2I14                414
                     *   INTTAUD2I15                415
                     */
                    { .pa = 0xFFBF6000, .va = 0xFFBF6000, .size = 0x400,
                      .interrupt_num = 16,
                      .interrupts = (irqid_t[]){ 400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415 }, },
                    /* TAUJ0   FFBF 7000H..FFBF 70FFH
                     *   INTTAUJ0I0                 360
                     *   INTTAUJ0I1                 361
                     *   INTTAUJ0I2                 362
                     *   INTTAUJ0I3                 363
                     */
                    { .pa = 0xFFBF7000, .va = 0xFFBF7000, .size = 0x100,
                      .interrupt_num = 4,
                      .interrupts = (irqid_t[]){ 360, 361, 362, 363 }, },
                    /* TAUJ1   FFBF 7200H..FFBF 72FFH
                     *   INTTAUJ1I0                 364
                     *   INTTAUJ1I1                 365
                     *   INTTAUJ1I2                 366
                     *   INTTAUJ1I3                 367
                     */
                    { .pa = 0xFFBF7200, .va = 0xFFBF7200, .size = 0x100,
                      .interrupt_num = 4,
                      .interrupts = (irqid_t[]){ 364, 365, 366, 367 }, },
                    /* TAPA0   FFBF 9000H..FFBF 903FH */
                    { .pa = 0xFFBF9000, .va = 0xFFBF9000, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* TAPA1   FFBF 9100H..FFBF 913FH */
                    { .pa = 0xFFBF9100, .va = 0xFFBF9100, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* TAPA2   FFBF 9200H..FFBF 923FH */
                    { .pa = 0xFFBF9200, .va = 0xFFBF9200, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* TAPA3   FFBF 9300H..FFBF 933FH */
                    { .pa = 0xFFBF9300, .va = 0xFFBF9300, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* TAUJ2   FFE8 0000H..FFE8 00FFH
                     *   INTTAUJ2I0                 368
                     *   INTTAUJ2I1                 369
                     *   INTTAUJ2I2                 370
                     *   INTTAUJ2I3                 371
                     */
                    { .pa = 0xFFE80000, .va = 0xFFE80000, .size = 0x100,
                      .interrupt_num = 4,
                      .interrupts = (irqid_t[]){ 368, 369, 370, 371 }, },
                    /* TAUJ3   FFE8 1000H..FFE8 10FFH
                     *   INTTAUJ3I0                 372
                     *   INTTAUJ3I1                 373
                     *   INTTAUJ3I2                 374
                     *   INTTAUJ3I3                 375
                     */
                    { .pa = 0xFFE81000, .va = 0xFFE81000, .size = 0x100,
                      .interrupt_num = 4,
                      .interrupts = (irqid_t[]){ 372, 373, 374, 375 }, },

                    /* ---- PWM ------------------------------------------------------- */
                    /* PWGC0   FFF0 0000H..FFF0 003FH */
                    { .pa = 0xFFF00000, .va = 0xFFF00000, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC1   FFF0 0200H..FFF0 023FH */
                    { .pa = 0xFFF00200, .va = 0xFFF00200, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC2   FFF0 0400H..FFF0 043FH */
                    { .pa = 0xFFF00400, .va = 0xFFF00400, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC3   FFF0 0600H..FFF0 063FH */
                    { .pa = 0xFFF00600, .va = 0xFFF00600, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC4   FFF0 0800H..FFF0 083FH */
                    { .pa = 0xFFF00800, .va = 0xFFF00800, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC5   FFF0 0A00H..FFF0 0A3FH */
                    { .pa = 0xFFF00A00, .va = 0xFFF00A00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC6   FFF0 0C00H..FFF0 0C3FH */
                    { .pa = 0xFFF00C00, .va = 0xFFF00C00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC7   FFF0 0E00H..FFF0 0E3FH */
                    { .pa = 0xFFF00E00, .va = 0xFFF00E00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC8   FFF0 1000H..FFF0 103FH */
                    { .pa = 0xFFF01000, .va = 0xFFF01000, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC9   FFF0 1200H..FFF0 123FH */
                    { .pa = 0xFFF01200, .va = 0xFFF01200, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC10   FFF0 1400H..FFF0 143FH */
                    { .pa = 0xFFF01400, .va = 0xFFF01400, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC11   FFF0 1600H..FFF0 163FH */
                    { .pa = 0xFFF01600, .va = 0xFFF01600, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC12   FFF0 1800H..FFF0 183FH */
                    { .pa = 0xFFF01800, .va = 0xFFF01800, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC13   FFF0 1A00H..FFF0 1A3FH */
                    { .pa = 0xFFF01A00, .va = 0xFFF01A00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC14   FFF0 1C00H..FFF0 1C3FH */
                    { .pa = 0xFFF01C00, .va = 0xFFF01C00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC15   FFF0 1E00H..FFF0 1E3FH */
                    { .pa = 0xFFF01E00, .va = 0xFFF01E00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC16   FFF0 2000H..FFF0 203FH */
                    { .pa = 0xFFF02000, .va = 0xFFF02000, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC17   FFF0 2200H..FFF0 223FH */
                    { .pa = 0xFFF02200, .va = 0xFFF02200, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC18   FFF0 2400H..FFF0 243FH */
                    { .pa = 0xFFF02400, .va = 0xFFF02400, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC19   FFF0 2600H..FFF0 263FH */
                    { .pa = 0xFFF02600, .va = 0xFFF02600, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC20   FFF0 2800H..FFF0 283FH */
                    { .pa = 0xFFF02800, .va = 0xFFF02800, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC21   FFF0 2A00H..FFF0 2A3FH */
                    { .pa = 0xFFF02A00, .va = 0xFFF02A00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC22   FFF0 2C00H..FFF0 2C3FH */
                    { .pa = 0xFFF02C00, .va = 0xFFF02C00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC23   FFF0 2E00H..FFF0 2E3FH */
                    { .pa = 0xFFF02E00, .va = 0xFFF02E00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC24   FFF0 3000H..FFF0 303FH */
                    { .pa = 0xFFF03000, .va = 0xFFF03000, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC25   FFF0 3200H..FFF0 323FH */
                    { .pa = 0xFFF03200, .va = 0xFFF03200, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC26   FFF0 3400H..FFF0 343FH */
                    { .pa = 0xFFF03400, .va = 0xFFF03400, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC27   FFF0 3600H..FFF0 363FH */
                    { .pa = 0xFFF03600, .va = 0xFFF03600, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC28   FFF0 3800H..FFF0 383FH */
                    { .pa = 0xFFF03800, .va = 0xFFF03800, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC29   FFF0 3A00H..FFF0 3A3FH */
                    { .pa = 0xFFF03A00, .va = 0xFFF03A00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC30   FFF0 3C00H..FFF0 3C3FH */
                    { .pa = 0xFFF03C00, .va = 0xFFF03C00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC31   FFF0 3E00H..FFF0 3E3FH */
                    { .pa = 0xFFF03E00, .va = 0xFFF03E00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC32   FFF0 4000H..FFF0 403FH */
                    { .pa = 0xFFF04000, .va = 0xFFF04000, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC33   FFF0 4200H..FFF0 423FH */
                    { .pa = 0xFFF04200, .va = 0xFFF04200, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC34   FFF0 4400H..FFF0 443FH */
                    { .pa = 0xFFF04400, .va = 0xFFF04400, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC35   FFF0 4600H..FFF0 463FH */
                    { .pa = 0xFFF04600, .va = 0xFFF04600, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC36   FFF0 4800H..FFF0 483FH */
                    { .pa = 0xFFF04800, .va = 0xFFF04800, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC37   FFF0 4A00H..FFF0 4A3FH */
                    { .pa = 0xFFF04A00, .va = 0xFFF04A00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC38   FFF0 4C00H..FFF0 4C3FH */
                    { .pa = 0xFFF04C00, .va = 0xFFF04C00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC39   FFF0 4E00H..FFF0 4E3FH */
                    { .pa = 0xFFF04E00, .va = 0xFFF04E00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC40   FFF0 5000H..FFF0 503FH */
                    { .pa = 0xFFF05000, .va = 0xFFF05000, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC41   FFF0 5200H..FFF0 523FH */
                    { .pa = 0xFFF05200, .va = 0xFFF05200, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC42   FFF0 5400H..FFF0 543FH */
                    { .pa = 0xFFF05400, .va = 0xFFF05400, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC43   FFF0 5600H..FFF0 563FH */
                    { .pa = 0xFFF05600, .va = 0xFFF05600, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC44   FFF0 5800H..FFF0 583FH */
                    { .pa = 0xFFF05800, .va = 0xFFF05800, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC45   FFF0 5A00H..FFF0 5A3FH */
                    { .pa = 0xFFF05A00, .va = 0xFFF05A00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC46   FFF0 5C00H..FFF0 5C3FH */
                    { .pa = 0xFFF05C00, .va = 0xFFF05C00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC47   FFF0 5E00H..FFF0 5E3FH */
                    { .pa = 0xFFF05E00, .va = 0xFFF05E00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC48   FFF0 6000H..FFF0 603FH */
                    { .pa = 0xFFF06000, .va = 0xFFF06000, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC49   FFF0 6200H..FFF0 623FH */
                    { .pa = 0xFFF06200, .va = 0xFFF06200, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC50   FFF0 6400H..FFF0 643FH */
                    { .pa = 0xFFF06400, .va = 0xFFF06400, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC51   FFF0 6600H..FFF0 663FH */
                    { .pa = 0xFFF06600, .va = 0xFFF06600, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC52   FFF0 6800H..FFF0 683FH */
                    { .pa = 0xFFF06800, .va = 0xFFF06800, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC53   FFF0 6A00H..FFF0 6A3FH */
                    { .pa = 0xFFF06A00, .va = 0xFFF06A00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC54   FFF0 6C00H..FFF0 6C3FH */
                    { .pa = 0xFFF06C00, .va = 0xFFF06C00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC55   FFF0 6E00H..FFF0 6E3FH */
                    { .pa = 0xFFF06E00, .va = 0xFFF06E00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC56   FFF0 7000H..FFF0 703FH */
                    { .pa = 0xFFF07000, .va = 0xFFF07000, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC57   FFF0 7200H..FFF0 723FH */
                    { .pa = 0xFFF07200, .va = 0xFFF07200, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC58   FFF0 7400H..FFF0 743FH */
                    { .pa = 0xFFF07400, .va = 0xFFF07400, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC59   FFF0 7600H..FFF0 763FH */
                    { .pa = 0xFFF07600, .va = 0xFFF07600, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC60   FFF0 7800H..FFF0 783FH */
                    { .pa = 0xFFF07800, .va = 0xFFF07800, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC61   FFF0 7A00H..FFF0 7A3FH */
                    { .pa = 0xFFF07A00, .va = 0xFFF07A00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC62   FFF0 7C00H..FFF0 7C3FH */
                    { .pa = 0xFFF07C00, .va = 0xFFF07C00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC63   FFF0 7E00H..FFF0 7E3FH */
                    { .pa = 0xFFF07E00, .va = 0xFFF07E00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC64   FFF0 8000H..FFF0 803FH */
                    { .pa = 0xFFF08000, .va = 0xFFF08000, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC65   FFF0 8200H..FFF0 823FH */
                    { .pa = 0xFFF08200, .va = 0xFFF08200, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC66   FFF0 8400H..FFF0 843FH */
                    { .pa = 0xFFF08400, .va = 0xFFF08400, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC67   FFF0 8600H..FFF0 863FH */
                    { .pa = 0xFFF08600, .va = 0xFFF08600, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC68   FFF0 8800H..FFF0 883FH */
                    { .pa = 0xFFF08800, .va = 0xFFF08800, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC69   FFF0 8A00H..FFF0 8A3FH */
                    { .pa = 0xFFF08A00, .va = 0xFFF08A00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC70   FFF0 8C00H..FFF0 8C3FH */
                    { .pa = 0xFFF08C00, .va = 0xFFF08C00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC71   FFF0 8E00H..FFF0 8E3FH */
                    { .pa = 0xFFF08E00, .va = 0xFFF08E00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC72   FFF0 9000H..FFF0 903FH */
                    { .pa = 0xFFF09000, .va = 0xFFF09000, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC73   FFF0 9200H..FFF0 923FH */
                    { .pa = 0xFFF09200, .va = 0xFFF09200, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC74   FFF0 9400H..FFF0 943FH */
                    { .pa = 0xFFF09400, .va = 0xFFF09400, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC75   FFF0 9600H..FFF0 963FH */
                    { .pa = 0xFFF09600, .va = 0xFFF09600, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC76   FFF0 9800H..FFF0 983FH */
                    { .pa = 0xFFF09800, .va = 0xFFF09800, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC77   FFF0 9A00H..FFF0 9A3FH */
                    { .pa = 0xFFF09A00, .va = 0xFFF09A00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC78   FFF0 9C00H..FFF0 9C3FH */
                    { .pa = 0xFFF09C00, .va = 0xFFF09C00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC79   FFF0 9E00H..FFF0 9E3FH */
                    { .pa = 0xFFF09E00, .va = 0xFFF09E00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC80   FFF0 A000H..FFF0 A03FH */
                    { .pa = 0xFFF0A000, .va = 0xFFF0A000, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC81   FFF0 A200H..FFF0 A23FH */
                    { .pa = 0xFFF0A200, .va = 0xFFF0A200, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC82   FFF0 A400H..FFF0 A43FH */
                    { .pa = 0xFFF0A400, .va = 0xFFF0A400, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC83   FFF0 A600H..FFF0 A63FH */
                    { .pa = 0xFFF0A600, .va = 0xFFF0A600, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC84   FFF0 A800H..FFF0 A83FH */
                    { .pa = 0xFFF0A800, .va = 0xFFF0A800, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC85   FFF0 AA00H..FFF0 AA3FH */
                    { .pa = 0xFFF0AA00, .va = 0xFFF0AA00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC86   FFF0 AC00H..FFF0 AC3FH */
                    { .pa = 0xFFF0AC00, .va = 0xFFF0AC00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC87   FFF0 AE00H..FFF0 AE3FH */
                    { .pa = 0xFFF0AE00, .va = 0xFFF0AE00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC88   FFF0 B000H..FFF0 B03FH */
                    { .pa = 0xFFF0B000, .va = 0xFFF0B000, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC89   FFF0 B200H..FFF0 B23FH */
                    { .pa = 0xFFF0B200, .va = 0xFFF0B200, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC90   FFF0 B400H..FFF0 B43FH */
                    { .pa = 0xFFF0B400, .va = 0xFFF0B400, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC91   FFF0 B600H..FFF0 B63FH */
                    { .pa = 0xFFF0B600, .va = 0xFFF0B600, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC92   FFF0 B800H..FFF0 B83FH */
                    { .pa = 0xFFF0B800, .va = 0xFFF0B800, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC93   FFF0 BA00H..FFF0 BA3FH */
                    { .pa = 0xFFF0BA00, .va = 0xFFF0BA00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC94   FFF0 BC00H..FFF0 BC3FH */
                    { .pa = 0xFFF0BC00, .va = 0xFFF0BC00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGC95   FFF0 BE00H..FFF0 BE3FH */
                    { .pa = 0xFFF0BE00, .va = 0xFFF0BE00, .size = 0x40,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGCINTF   FFF0 C200H..FFF0 C20FH */
                    { .pa = 0xFFF0C200, .va = 0xFFF0C200, .size = 0x10,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGCINTF   FFF0 C400H..FFF0 C40FH */
                    { .pa = 0xFFF0C400, .va = 0xFFF0C400, .size = 0x10,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGCINTF   FFF0 C600H..FFF0 C60FH */
                    { .pa = 0xFFF0C600, .va = 0xFFF0C600, .size = 0x10,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGCINTF   FFF0 C800H..FFF0 C80FH */
                    { .pa = 0xFFF0C800, .va = 0xFFF0C800, .size = 0x10,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGCINTF   FFF0 CA00H..FFF0 CA0FH */
                    { .pa = 0xFFF0CA00, .va = 0xFFF0CA00, .size = 0x10,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGCINTF   FFF0 CC00H..FFF0 CC0FH */
                    { .pa = 0xFFF0CC00, .va = 0xFFF0CC00, .size = 0x10,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGCINTF   FFF0 CE00H..FFF0 CE0FH */
                    { .pa = 0xFFF0CE00, .va = 0xFFF0CE00, .size = 0x10,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGCINTF   FFF0 D000H..FFF0 D00FH */
                    { .pa = 0xFFF0D000, .va = 0xFFF0D000, .size = 0x10,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGCINTF   FFF0 D200H..FFF0 D20FH */
                    { .pa = 0xFFF0D200, .va = 0xFFF0D200, .size = 0x10,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGCINTF   FFF0 D400H..FFF0 D40FH */
                    { .pa = 0xFFF0D400, .va = 0xFFF0D400, .size = 0x10,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGCINTF   FFF0 D600H..FFF0 D60FH */
                    { .pa = 0xFFF0D600, .va = 0xFFF0D600, .size = 0x10,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PWGCINTF   FFF0 D800H..FFF0 D80FH */
                    { .pa = 0xFFF0D800, .va = 0xFFF0D800, .size = 0x10,
                      .interrupt_num = 0, .interrupts = NULL, },

                    /* ---- ADC ------------------------------------------------------- */
                    /* ADCJ2   FF9A 2000H..FF9A 2FFFH */
                    { .pa = 0xFF9A2000, .va = 0xFF9A2000, .size = 0x1000,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* ADCJ2_SELB   FF9A 3800H..FF9A 381FH */
                    { .pa = 0xFF9A3800, .va = 0xFF9A3800, .size = 0x20,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* ADCJ0   FFCA 0000H..FFCA 0FFFH */
                    { .pa = 0xFFCA0000, .va = 0xFFCA0000, .size = 0x1000,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* ADCJ1   FFF2 0000H..FFF2 0FFFH */
                    { .pa = 0xFFF20000, .va = 0xFFF20000, .size = 0x1000,
                      .interrupt_num = 0, .interrupts = NULL, },

                    /* ---- Watchdog -------------------------------------------------- */
                    /* WDTBA   FF9A 5000H..FF9A 501FH */
                    { .pa = 0xFF9A5000, .va = 0xFF9A5000, .size = 0x20,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* WDTBAOPBMON   FF9A 5200H..FF9A 520FH */
                    { .pa = 0xFF9A5200, .va = 0xFF9A5200, .size = 0x10,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* WDTB0   FFBF 1000H..FFBF 101FH */
                    { .pa = 0xFFBF1000, .va = 0xFFBF1000, .size = 0x20,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* WDTB1   FFBF 1100H..FFBF 111FH */
                    { .pa = 0xFFBF1100, .va = 0xFFBF1100, .size = 0x20,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* WDTB2   FFBF 1200H..FFBF 121FH */
                    { .pa = 0xFFBF1200, .va = 0xFFBF1200, .size = 0x20,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* WDTB3   FFBF 1300H..FFBF 131FH */
                    { .pa = 0xFFBF1300, .va = 0xFFBF1300, .size = 0x20,
                      .interrupt_num = 0, .interrupts = NULL, },

                    /* ---- CRC ------------------------------------------------------- */
                    /* KCRC1   FFBF F400H..FFBF F4FFH */
                    { .pa = 0xFFBFF400, .va = 0xFFBFF400, .size = 0x100,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* KCRC3   FFBF F700H..FFBF F7FFH */
                    { .pa = 0xFFBFF700, .va = 0xFFBFF700, .size = 0x100,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* KCRC5   FFBF FA00H..FFBF FAFFH */
                    { .pa = 0xFFBFFA00, .va = 0xFFBFFA00, .size = 0x100,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* KCRC7   FFBF FD00H..FFBF FDFFH */
                    { .pa = 0xFFBFFD00, .va = 0xFFBFFD00, .size = 0x100,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* KCRC0   FFD3 4000H..FFD3 40FFH */
                    { .pa = 0xFFD34000, .va = 0xFFD34000, .size = 0x100,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* KCRC2   FFD3 5000H..FFD3 50FFH */
                    { .pa = 0xFFD35000, .va = 0xFFD35000, .size = 0x100,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* KCRC4   FFD3 6000H..FFD3 60FFH */
                    { .pa = 0xFFD36000, .va = 0xFFD36000, .size = 0x100,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* KCRC6   FFD3 7000H..FFD3 70FFH */
                    { .pa = 0xFFD37000, .va = 0xFFD37000, .size = 0x100,
                      .interrupt_num = 0, .interrupts = NULL, },

                    /* ---- Interrupt ------------------------------------------------- */
                    /* INTC2   FFF8 0000H..FFF8 7FFFH */
                    { .pa = 0xFFF80000, .va = 0xFFF80000, .size = 0x8000,
                      .interrupt_num = 0, .interrupts = NULL, },

                    /* ---- Boot ------------------------------------------------------ */
                    /* BOOTCTRL   FFFB 2000H..FFFB 200FH */
                    { .pa = 0xFFFB2000, .va = 0xFFFB2000, .size = 0x10,
                      .interrupt_num = 0, .interrupts = NULL, },

                    /* ---- Peripheral interconnect ----------------------------------- */
                    /* PIC1   FFBF AF00H..FFBF AFFFH */
                    { .pa = 0xFFBFAF00, .va = 0xFFBFAF00, .size = 0x100,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PIC1   FFBF B200H..FFBF B203H */
                    { .pa = 0xFFBFB200, .va = 0xFFBFB200, .size = 0x4,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PIC1   FFBF B400H..FFBF B403H */
                    { .pa = 0xFFBFB400, .va = 0xFFBFB400, .size = 0x4,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PIC1   FFBF B600H..FFBF B603H */
                    { .pa = 0xFFBFB600, .va = 0xFFBFB600, .size = 0x4,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PIC20   FFBF C000H..FFBF C7FFH */
                    { .pa = 0xFFBFC000, .va = 0xFFBFC000, .size = 0x800,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PIC20   FFBF CA00H..FFBF CA03H */
                    { .pa = 0xFFBFCA00, .va = 0xFFBFCA00, .size = 0x4,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PIC21   FFBF D000H..FFBF D7FFH */
                    { .pa = 0xFFBFD000, .va = 0xFFBFD000, .size = 0x800,
                      .interrupt_num = 0, .interrupts = NULL, },
                    /* PIC22   FFBF E000H..FFBF E0FFH */
                    { .pa = 0xFFBFE000, .va = 0xFFBFE000, .size = 0x100,
                      .interrupt_num = 0, .interrupts = NULL, },
                },
            },
        },
    },
};
