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
            .image = VM_IMAGE_LOADED(0x10000,0x10000,0x100000),
            .cpu_affinity = 1,

            .platform = {
                .cpu_num = 1,
                .region_num = 2,
                .regions =  (struct vm_mem_region[]) {
                    // Guest code
                    {
                        .base = 0x10000,
                        .size = 0x100000
                    },
                    // Guest retention RAM (Cluster3 RAM)
                    {
                        .base = 0xfe800000,
                        .size = 0x40000
                    }
                },

                .dev_num = 4,
                .devs =  (struct vm_dev_region[]) {
                    // Local RAM
                    {
                        // TODO: This is actually guest memory. We must find a way to deal with self regions
                        // 0xFDE00000 -> 0xFDE10000
                        .pa = 0xfde00000,
                        .va = 0xfde00000,
                        .size = 0x10000,
                        .interrupt_num = 0,
                        .interrupts = NULL
                    },
                    // TAUJ1
                    {
                        // 0xFFBF7200 -> 0xFFBF7300
                        .pa = 0xFFBF7200,
                        .va = 0xFFBF7200,
                        .size = 0x100,
                        .interrupt_num = 1,
                        .interrupts = (irqid_t[]) {364}
                    },
                    // OSTM1
                    {
                        // 0xFFBF0100 -> 0xFFBF0140
                        .pa = 0xFFBF0100,
                        .va = 0xFFBF0100,
                        .size = 0x40,
                        .interrupt_num = 1,
                        .interrupts = (irqid_t[]) {200}
                    },
                    // RSCFD0
                    {
                        // 0xFFF50000 -> 0xFFF70000
                        .pa = 0xFFF50000,
                        .va = 0xFFF50000,
                        .size = 0x20000,
                        .interrupt_num = 7,
                        .interrupts = (irqid_t[]) {297, 304, 305, 306, 319, 320, 321}
                    }
                }
            }
        }
    }
};
