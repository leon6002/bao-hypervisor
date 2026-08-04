/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

/* THIS CONFIG IS DEFINED FOR TESTING PURPOSES ONLY */

#include <config.h>

struct config config = {

    /**
     * This configuration has no VMs. It is used to test build the 
     * hypervisor with an empty configuration. We have to set
     * `vmlist_size` to 1, because the build scripts will generate
     * a macro to define an array size from it which automatically
     * triggers a set of `array subscript i is outside array bounds`
     * errors and the build which we are testing in the first place
     *  will fail.
     */
    .vmlist_size = 1,

    .vmlist = (struct vm_config[]){ 
        {
            .entry = 0x10000,
            .image = VM_IMAGE_LOADED(0x10000,0x10000,0x10000),
            .cpu_affinity = 1,

            .platform = {
                .cpu_num = 1,
                .region_num = 2,
                .regions =  (struct vm_mem_region[]) {
                    // Code Flash (Bank A) -> Guest code
                    {
                        .base = 0x10000,
                        .size = 0x10000
                    },
                    // Cluster0 RAM -> Guest Data
                    {
                        .base = 0xfe000000,
                        .size = 0x80000
                    }
                },

                .dev_num = 5,
                .devs =  (struct vm_dev_region[]) {
                    // Standby Controller
                    {
                        // 0xFF981000 -> 0xFF982000
                        .pa = 0xFF981000,
                        .va = 0xFF981000,
                        .size = 0x1000,
                        .interrupt_num = 0,
                        .interrupts = NULL
                    },
                    // RLIN35
                    {
                        // 0xFFC7C100 -> 0xFFC7C140
                        .pa = 0xFFC7C100,
                        .va = 0xFFC7C100,
                        .size = 0x40,
                        .interrupt_num = 1,
                        .interrupts = (irqid_t[]) {438}
                    },
                    // INTIF
                    {
                        // 0xFF090000 -> 0xFF090220
                        .pa = 0xFF090000,
                        .va = 0xFF090000,
                        .size = 0x220,
                        .interrupt_num = 0,
                        .interrupts = NULL
                    },
                    // TPTM
                    {
                        // 0xFFFBB000 -> 0xFFFBC000
                        .pa = 0xFFFBB000,
                        .va = 0xFFFBB000,
                        .size = 0x1000,
                        .interrupt_num = 1,
                        .interrupts = (irqid_t[]) {209}
                    },
                    // INTC1 (self)
                    {
                        // 0xFFFC0000 -> 0xFFFC4000
                        .pa = 0xFFFC0000,
                        .va = 0xFFFC0000,
                        .size = 0x4000,
                        .interrupt_num = 0,
                        .interrupts = NULL
                    }
                }
            }
        }
    }
};
