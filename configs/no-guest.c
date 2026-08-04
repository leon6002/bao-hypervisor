/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

/* THIS CONFIG IS DEFINED FOR TESTING PURPOSES ONLY */

#include <config.h>

struct config config = {

    .vmlist_size = 1,

    .vmlist = (struct vm_config[]){
        {
            .entry = 0x2000,
            .image = {
                .load_addr = 0x0,
                .size = 0x1000
            },
            .platform = {
                .cpu_num = 1,
                .region_num = 1,
                .regions =  (struct vm_mem_region[]) {
                    {
                        .base = 0x0,
                        .size = 0x1000
                    }
                },
            }
        }
    },
};
