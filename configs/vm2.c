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
            .entry = 0x7F0000,
            .image = VM_IMAGE_LOADED(0x7F0000,0x7F0000,0x100000),
            .cpu_affinity = 1,

            .platform = {
                .cpu_num = 1,
                .region_num = 2,
                .regions =  (struct vm_mem_region[]) {
                    // Guest code
                    {
                        .base = 0x7F0000,
                        .size = 0x100000
                    },
                    // Guest retention RAM (Cluster3 RAM)
                    {
                        .base = 0xfe820000,
                        .size = 0x20000
                    }
                },

                .dev_num = 6,
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
                    // OSTM0
                    {
                        // 0xFFBF0000 -> 0xFFBF0040
                        .pa = 0xFFBF0000,
                        .va = 0xFFBF0000,
                        .size = 0x40,
                        .interrupt_num = 1,
                        .interrupts = (irqid_t[]) {199}
                    },
                    // TAUD1
                    {
                        // 0xFFBF5000 -> 0xFFBF5400
                        .pa = 0xFFBF5000,
                        .va = 0xFFBF5000,
                        .size = 0x400,
                        .interrupt_num = 4,
                        .interrupts = (irqid_t[]) {396, 397, 398, 399}
                    },
                    // TAUJ3
                    {
                        // 0xFFE81000 -> 0xFFE81100
                        .pa = 0xFFE81000,
                        .va = 0xFFE81000,
                        .size = 0x100,
                        .interrupt_num = 4,
                        .interrupts = (irqid_t[]) {372, 373, 374, 375}
                    },
                    // RLIN35
                    {
                        // 0xFFC7C100 -> 0xFFC7C140
                        .pa = 0xFFC7C100,
                        .va = 0xFFC7C100,
                        .size = 0x40,
                        .interrupt_num = 3,
                        .interrupts = (irqid_t[]) {437, 438, 439}
                    },
                    // RLIN34
                    {
                        // 0xFFD28400 -> 0xFFD28440
                        .pa = 0xFFD28400,
                        .va = 0xFFD28400,
                        .size = 0x40,
                        .interrupt_num = 3,
                        .interrupts = (irqid_t[]) {433, 434, 435}
                    }
                }
            }
        }
    }
};
