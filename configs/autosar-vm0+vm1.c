/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

/*
 * Two-VM AUTOSAR demo configuration for RH850/U2A.
 *
 * Recovered by decoding the `config` and `platform` structures out of the reference image
 * archive/releases/renesas/bao/bao_AutoSar_TwoVM.elf, which is the CC-RH build known to drive
 * both the UART guest and the CAN guest on real hardware. That image was built by Manuel
 * Rodriguez from shangai-odyssey-auto-demo with PLATFORM=rh850-u2a-autosar-vm0+vm1 and
 * CONFIG=autosar-vm0+vm1; neither the platform nor the config survived in any branch we hold,
 * so both are reconstructed here from the binary.
 *
 * This differs from twovm.c / vm1+vm2.c in ways that matter:
 *   - VM0 code window is 0x30000, not 0x100000.
 *   - VM0 RAM is 0xFE000000+0x80000; the shared region is 0xFE800000+0x1000. The two are
 *     swapped relative to twovm.c.
 *   - VM0 gets INTC1 (0xFFFC0000) mapped, which twovm.c does not, and does not get TAUJ1.
 */

#include <config.h>

struct config config = {

    .shmemlist_size = 1,
    .shmemlist = (struct shmem[]){
        [0] = {
            .base = 0xFE800000,
            .size = 0x1000,
        },
    },

    .vmlist_size = 2,

    .vmlist = (struct vm_config[]){
        /* VM0 -- AUTOSAR guest driving CAN, flashed at 0x10000 */
        {
            .entry = 0x10000,
            .image = VM_IMAGE_LOADED(0x10000, 0x10000, 0x30000),
            .cpu_affinity = 1,

            .platform = {
                .cpu_num = 1,

                .region_num = 2,
                .regions = (struct vm_mem_region[]){
                    /* Guest code */
                    {
                        .base = 0x10000,
                        .size = 0x30000,
                    },
                    /* Guest RAM */
                    {
                        .base = 0xFE000000,
                        .size = 0x80000,
                    },
                },

                .ipc_num = 1,
                .ipcs = (struct ipc[]){
                    {
                        .base = 0xFE800000,
                        .size = 0x1000,
                        .shmem_id = 0,
                        .interrupt_num = 0,
                        .interrupts = NULL,
                    },
                },

                .dev_num = 6,
                .devs = (struct vm_dev_region[]){
                    /* Local RAM */
                    {
                        .pa = 0xFDE00000,
                        .va = 0xFDE00000,
                        .size = 0x10000,
                        .interrupt_num = 0,
                        .interrupts = NULL,
                    },
                    /* Standby Controller */
                    {
                        .pa = 0xFF981000,
                        .va = 0xFF981000,
                        .size = 0x1000,
                        .interrupt_num = 0,
                        .interrupts = NULL,
                    },
                    /* OSTM1 */
                    {
                        .pa = 0xFFBF0100,
                        .va = 0xFFBF0100,
                        .size = 0x40,
                        .interrupt_num = 1,
                        .interrupts = (irqid_t[]){ 200 },
                    },
                    /* RSCFD0 -- the CAN controller */
                    {
                        .pa = 0xFFF50000,
                        .va = 0xFFF50000,
                        .size = 0x20000,
                        .interrupt_num = 8,
                        .interrupts = (irqid_t[]){ 296, 297, 304, 305, 306, 319, 320, 321 },
                    },
                    /* INTC1 */
                    {
                        .pa = 0xFFFC0000,
                        .va = 0xFFFC0000,
                        .size = 0x4000,
                        .interrupt_num = 0,
                        .interrupts = NULL,
                    },
                    {
                        .pa = 0xFF9A3B00,
                        .va = 0xFF9A3B00,
                        .size = 0x100,
                        .interrupt_num = 0,
                        .interrupts = NULL,
                    },
                },
            },
        },

        /* VM1 -- UART guest, flashed at 0x7F0000 */
        {
            .entry = 0x7F0000,
            .image = VM_IMAGE_LOADED(0x7F0000, 0x7F0000, 0x100000),
            .cpu_affinity = 2,

            .platform = {
                .cpu_num = 1,

                .region_num = 2,
                .regions = (struct vm_mem_region[]){
                    /* Guest code */
                    {
                        .base = 0x7F0000,
                        .size = 0x100000,
                    },
                    /* Guest RAM */
                    {
                        .base = 0xFE820000,
                        .size = 0x20000,
                    },
                },

                .ipc_num = 1,
                .ipcs = (struct ipc[]){
                    {
                        .base = 0xFE800000,
                        .size = 0x1000,
                        .shmem_id = 0,
                        .interrupt_num = 0,
                        .interrupts = NULL,
                    },
                },

                .dev_num = 6,
                .devs = (struct vm_dev_region[]){
                    /* Local RAM */
                    {
                        .pa = 0xFDE00000,
                        .va = 0xFDE00000,
                        .size = 0x10000,
                        .interrupt_num = 0,
                        .interrupts = NULL,
                    },
                    /* OSTM0 */
                    {
                        .pa = 0xFFBF0000,
                        .va = 0xFFBF0000,
                        .size = 0x40,
                        .interrupt_num = 1,
                        .interrupts = (irqid_t[]){ 199 },
                    },
                    {
                        .pa = 0xFFBF5000,
                        .va = 0xFFBF5000,
                        .size = 0x400,
                        .interrupt_num = 4,
                        .interrupts = (irqid_t[]){ 396, 397, 398, 399 },
                    },
                    {
                        .pa = 0xFFE81000,
                        .va = 0xFFE81000,
                        .size = 0x100,
                        .interrupt_num = 4,
                        .interrupts = (irqid_t[]){ 372, 373, 374, 375 },
                    },
                    /* RLIN35 -- the guest console */
                    {
                        .pa = 0xFFC7C100,
                        .va = 0xFFC7C100,
                        .size = 0x40,
                        .interrupt_num = 3,
                        .interrupts = (irqid_t[]){ 437, 438, 439 },
                    },
                    {
                        .pa = 0xFFD28400,
                        .va = 0xFFD28400,
                        .size = 0x40,
                        .interrupt_num = 3,
                        .interrupts = (irqid_t[]){ 433, 434, 435 },
                    },
                },
            },
        },
    },
};
