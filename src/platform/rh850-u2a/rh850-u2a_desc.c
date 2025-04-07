/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <platform.h>

irqid_t plat_ints[2048];

struct platform platform = {

    .cpu_num = 2,
    .cpu_master_fixed = true,
    .cpu_master = 0,

    .region_num = 8,
    .regions =
        (struct mem_region[]){
            // Cluster1 RAM -> Bao Data
            {
                .base = 0xfe100000,
                .size = 0x80000,
                .perms = RWX,
            },
            // Local RAM (self) -> Guest stack
            {
                .base = 0xfde00000,
                .size = 0x10000,
                .perms = RWX,
            },
            // Cluster0 RAM -> Guest Data
            {
                .base = 0xfe000000,
                .size = 0x80000,
                .perms = RWX,
            },
            // Cluster2 RAM -> Guest Data
            {
                .base = 0xfe400000,
                .size = 0x100000,
                .perms = RWX,
            },
            // Cluster3 RAM -> Guest retention RAM
            {
                .base = 0xfe800000,
                .size = 0x40000,
                .perms = RWX,
            },
            // Data Flash (HPA) -> Guest data flash
            {
                .base = 0xff320000,
                .size = 0x55000,
                .perms = RX,
            },
            // Code Flash (Bank A) -> Bao code
            {
                .base = 0x0,
                .size = 0x10000,
                .perms = RX,
            },
            // Code Flash (Bank B) -> Guest code
            {
                .base = 0x10000,
                .size = 0x100000,
                .perms = RX,
            }
        },

    .arch = {
        .intc = {
            .intc1_addr = 0xFFFC0000,   // mapped 1, mapped 0
            .intc2_addr = 0xFFF80000,
            .intif_addr = 0xFF090000,   // mapped 0, mapped 1
            .eint_addr = 0xFFC00000,
            .fenc_addr = 0xFF9A3A00,
            .feinc_addr = {
                0xFF9A3B00, // mapped 0
                0xFF9A3C00, // mapped 1 
                0xFF9A3D00,
                0xFF9A3E00,
            },
        },

        .bootctrl_addr = 0xFFFB2000,
        .ipir_addr = 0xFFFB9000 // mapped 0, mapped 1
    }
};
