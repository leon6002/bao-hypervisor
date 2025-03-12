/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __ARCH_MEM_H__
#define __ARCH_MEM_H__

#include <bao.h>

typedef union {
    struct {
        uint16_t ur : 1; // hyp att? 
        uint16_t uw : 1; // hyp att?
        uint16_t ux : 1; // hyp att?
        uint16_t sr : 1; // hyp att?
        uint16_t sw : 1; // hyp att?
        uint16_t sx : 1; // hyp att?
        uint16_t res1 : 1;
        uint16_t e : 1;
        uint16_t res2 : 6;
        uint16_t rg;
        uint16_t wg;
        uint16_t rmpid0: 1; // VM attributes?
        uint16_t rmpid1: 1; // VM attributes?
        uint16_t rmpid2: 1; // VM attributes?
        uint16_t rmpid3: 1; // VM attributes?
        uint16_t rmpid4: 1; // VM attributes?
        uint16_t rmpid5: 1; // VM attributes?
        uint16_t rmpid6: 1; // VM attributes?
        uint16_t rmpid7: 1; // VM attributes?
        uint16_t wmpid0: 1; // VM attributes?
        uint16_t wmpid1: 1; // VM attributes?
        uint16_t wmpid2: 1; // VM attributes?
        uint16_t wmpid3: 1; // VM attributes?
        uint16_t wmpid4: 1; // VM attributes?
        uint16_t wmpid5: 1; // VM attributes?
        uint16_t wmpid6: 1; // VM attributes?
        uint16_t wmpid7: 1; // VM attributes?
    };
    uint32_t raw;
} mpat_flags_t;


typedef unsigned long mem_flags_t;

#define PTE_FLAGS(at)     ((mem_flags_t)at)

#define PTE_INVALID       PTE_FLAGS(0)

#define PTE_HYP_FLAGS     PTE_FLAGS(0)
#define PTE_HYP_DEV_FLAGS PTE_FLAGS(0)
// TODO:ARMV8M - We are missing flags to distinguish flags for mem regions RX or RW

#define PTE_VM_FLAGS      PTE_FLAGS(0)
#define PTE_VM_DEV_FLAGS  PTE_FLAGS(0)

#define MPU_ARCH_MAX_NUM_ENTRIES (32)


static inline size_t mpu_granularity(void)
{
    return (size_t)PAGE_SIZE;
}

#endif /* __ARCH_MEM_H__ */
