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

    .region_num = 7,
    .regions =
        (struct mem_region[]){
            // Bao Data
            {
                .base = 0xfe100000,
                .size = 0x80000,
                .perms = RWX,
            },
            // Guest1 Data
            {
                .base = 0xfe000000,
                .size = 0x80000,
                .perms = RWX,
            },
            // Guest2 Data
            {
                .base = 0xfe400000,
                .size = 0x80000,
                .perms = RWX,
            },
            // Shared Region
            {
                .base = 0xfe800000,
                .size = 0x1000,
                .perms = RWX,
            },
            // Bao code
            {
                .base = 0x0,
                .size = 0x10000,
                .perms = RX,
            },
            // Guest1 code
            {
                .base = 0x10000,
                .size = 0x10000,
                .perms = RX,
            },
            // Guest2 code
            {
                .base = 0x20000,
                .size = 0x10000,
                .perms = RX,
            }
        },

    .console = {
        .base = 0xFFC7C100, // RLIN35
    },

    .arch = {
        .intc = {
            .intc1_addr = 0xFFFC0000,
            .intc2_addr = 0xFFF80000,
            .intif_addr = 0xFF090000,
            .eint_addr = 0xFFC00000,
            .fenc_addr = 0xFF9A3A00,
            .feinc_addr = {
                0xFF9A3B00,
                0xFF9A3C00, 
                0xFF9A3D00,
                0xFF9A3E00,
            },
        },

        .bootctrl_addr = 0xFFFB2000,
        .ipir_addr = 0xFFFB9000
    }
};
