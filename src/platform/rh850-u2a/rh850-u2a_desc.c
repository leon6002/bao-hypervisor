/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <platform.h>

irqid_t plat_ints[2048];

struct platform platform = {

    .cpu_num = 4,
    .cpu_master_fixed = true,
    .cpu_master = 0,

    .region_num = 1,
    .regions =
        (struct mem_region[]){
            {
                .base = 0x30020000,
                .size = 0x20000,
                .perms = RWX,
            },
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

        .ipir_addr = 0xFFFB9000,
    }
};
