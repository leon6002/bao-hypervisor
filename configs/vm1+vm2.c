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
            .base = 0xfe000000,
            .size = 0x1000
        }
    },

    .vmlist_size = 2,

    .vmlist = (struct vm_config[]){ 
        // VM1_CAN1_0x10000_WithoutCLK
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

                .ipc_num = 1,
                .ipcs = (struct ipc[]) {
                    {
                        .base = 0xfe000000,
                        .size = 0x1000,
                        .shmem_id = 0,
                        .interrupt_num = 0,
                        .interrupts = NULL
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
        },
        // VM2_Uart_0x7F0000_WithoutCLK
        {
            .entry = 0x7F0000,
            .image = VM_IMAGE_LOADED(0x7F0000,0x7F0000,0x100000),
            .cpu_affinity = 2,

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

                .ipc_num = 1,
                .ipcs = (struct ipc[]) {
                    {
                        .base = 0xfe000000,
                        .size = 0x1000,
                        .shmem_id = 0,
                        .interrupt_num = 0,
                        .interrupts = NULL
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
