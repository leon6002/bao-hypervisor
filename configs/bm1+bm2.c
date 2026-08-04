/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

/* THIS CONFIG IS DEFINED FOR TESTING PURPOSES ONLY */

#include <config.h>

struct config config = {

    .shmemlist_size = 1,
    .shmemlist = (struct shmem[]) {
        [0] = { 
            .base = 0xfe800000,
            .size = 0x1000
        }
    },

    .vmlist_size = 2,

    .vmlist = (struct vm_config[]){
        // Baremetal w/ UART5 + TPTM
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

                .ipc_num = 1,
                .ipcs = (struct ipc[]) {
                    {
                        .base = 0xfe800000,
                        .size = 0x1000,
                        .shmem_id = 0,
                        .interrupt_num = 0,
                        .interrupts = NULL
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
        },
        // Baremetal w/ UART4
        {
            .entry = 0x20000,
            .image = VM_IMAGE_LOADED(0x20000,0x20000,0x100000),
            .cpu_affinity = 2,

            .platform = {
                .cpu_num = 1,
                .region_num = 2,
                .regions =  (struct vm_mem_region[]) {
                    // Code Flash (Bank A) -> Guest code
                    {
                        .base = 0x20000,
                        .size = 0x10000
                    },
                    // Cluster2 RAM -> Guest Data
                    {
                        .base = 0xfe400000,
                        .size = 0x80000,
                    },
                },

                .ipc_num = 1,
                .ipcs = (struct ipc[]) {
                    {
                        .base = 0xfe800000,
                        .size = 0x1000,
                        .shmem_id = 0,
                        .interrupt_num = 0,
                        .interrupts = NULL
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
                    // RLIN34
                    {
                        // 0xFFD28400 -> 0xFFD28440
                        .pa = 0xFFD28400,
                        .va = 0xFFD28400,
                        .size = 0x40,
                        .interrupt_num = 1,
                        .interrupts = (irqid_t[]) {434}
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
                        .interrupts = (irqid_t[]) {213}
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
