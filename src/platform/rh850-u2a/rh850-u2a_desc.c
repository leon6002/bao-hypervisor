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

    .region_num = 4,
    .regions =
        (struct mem_region[]){
            {
                .base = 0x30020000,
                .size = 0x20000,
                .perms = RWX,
            },

        },
};
