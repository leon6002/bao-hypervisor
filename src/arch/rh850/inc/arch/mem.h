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
        uint16_t rg : 1;
        uint16_t wg : 1;
        uint16_t rmpid0 : 1; // VM attributes?
        uint16_t rmpid1 : 1; // VM attributes?
        uint16_t rmpid2 : 1; // VM attributes?
        uint16_t rmpid3 : 1; // VM attributes?
        uint16_t rmpid4 : 1; // VM attributes?
        uint16_t rmpid5 : 1; // VM attributes?
        uint16_t rmpid6 : 1; // VM attributes?
        uint16_t rmpid7 : 1; // VM attributes?
        uint16_t wmpid0 : 1; // VM attributes?
        uint16_t wmpid1 : 1; // VM attributes?
        uint16_t wmpid2 : 1; // VM attributes?
        uint16_t wmpid3 : 1; // VM attributes?
        uint16_t wmpid4 : 1; // VM attributes?
        uint16_t wmpid5 : 1; // VM attributes?
        uint16_t wmpid6 : 1; // VM attributes?
        uint16_t wmpid7 : 1; // VM attributes?
    };
    uint32_t raw;
} mpat_flags_t;

typedef mpat_flags_t mem_flags_t;

#define PTE_INVALID       ((mem_flags_t){ .e = 0 })

#define PTE_HYP_FLAGS     ((mem_flags_t){ .e = 1, .sr = 1, .sw = 1, .sx = 1, .rg = 1, .wg = 1 })
#define PTE_HYP_DEV_FLAGS ((mem_flags_t){ .e = 1, .sr = 1, .sw = 1, .sx = 0, .rg = 1, .wg = 1 })

/* TODO in the future we need to deal with IO permissions securely */
#define PTE_VM_FLAGS \
    ((mem_flags_t){ .e = 1, .sr = 1, .sw = 1, .sx = 1, .ur = 1, .uw = 1, .ux = 1, .rg = 1, .wg = 1 })

/* TODO in the future we need to deal with IO permissions securely */
#define PTE_VM_DEV_FLAGS \
    ((mem_flags_t){ .e = 1, .sr = 1, .sw = 1, .sx = 0, .ur = 1, .uw = 1, .ux = 0, .rg = 1, .wg = 1 })

#define MPU_ARCH_MAX_NUM_ENTRIES (32)

static inline size_t mpu_granularity(void)
{
    return (size_t)PAGE_SIZE;
}

#endif /* __ARCH_MEM_H__ */
