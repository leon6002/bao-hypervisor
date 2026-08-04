/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __ARCH_MEM_H__
#define __ARCH_MEM_H__

#include <bao.h>

#define MPU_ARCH_MAX_NUM_ENTRIES (32)

typedef union {
    struct {
        uint16_t ur : 1;
        uint16_t uw : 1;
        uint16_t ux : 1;
        uint16_t sr : 1;
        uint16_t sw : 1;
        uint16_t sx : 1;
        uint16_t res1 : 1;
        uint16_t e : 1;
        uint16_t res2 : 6;
        uint16_t rg : 1;
        uint16_t wg : 1;
        uint16_t rmpid0 : 1;
        uint16_t rmpid1 : 1;
        uint16_t rmpid2 : 1;
        uint16_t rmpid3 : 1;
        uint16_t rmpid4 : 1;
        uint16_t rmpid5 : 1;
        uint16_t rmpid6 : 1;
        uint16_t rmpid7 : 1;
        uint16_t wmpid0 : 1;
        uint16_t wmpid1 : 1;
        uint16_t wmpid2 : 1;
        uint16_t wmpid3 : 1;
        uint16_t wmpid4 : 1;
        uint16_t wmpid5 : 1;
        uint16_t wmpid6 : 1;
        uint16_t wmpid7 : 1;
    };
    uint32_t raw;
} mpat_flags_t;

typedef mpat_flags_t mem_flags_t;

/*
 * The permission constants are composed through .raw rather than through the bit field above.
 * CC-RH rejects designated initializers for members of an anonymous struct or union
 * (E0523134), and naming the field instead makes it store the value field by field at every
 * use site rather than folding it into one word. The bit macros keep the meaning visible
 * without either cost; they mirror the layout above, so keep the two in sync.
 */
#define MPAT_UR       (1UL << 0)
#define MPAT_UW       (1UL << 1)
#define MPAT_UX       (1UL << 2)
#define MPAT_SR       (1UL << 3)
#define MPAT_SW       (1UL << 4)
#define MPAT_SX       (1UL << 5)
#define MPAT_E        (1UL << 7)
#define MPAT_RG       (1UL << 14)
#define MPAT_WG       (1UL << 15)
#define MPAT_RMPID(n) (1UL << (16 + (n)))
#define MPAT_WMPID(n) (1UL << (24 + (n)))

#define PTE_INVALID   ((mem_flags_t){ .raw = 0 })

/* Only the SPID in MPID7 can read and execute this region */
#define PTE_HYP_FLAGS_CODE                                                            \
    ((mem_flags_t){ .raw = MPAT_UR | MPAT_UX | MPAT_SR | MPAT_SX | MPAT_E |           \
              MPAT_RMPID(7) | MPAT_WMPID(7) })

/* Only the SPID in MPID7 can read and write this region */
#define PTE_HYP_FLAGS                                                                 \
    ((mem_flags_t){ .raw = MPAT_UR | MPAT_UW | MPAT_SR | MPAT_SW | MPAT_E |           \
              MPAT_RMPID(7) | MPAT_WMPID(7) })

/* Only the SPID in MPID7 can read and write this region */
#define PTE_HYP_DEV_FLAGS                                                             \
    ((mem_flags_t){ .raw = MPAT_UR | MPAT_UW | MPAT_SR | MPAT_SW | MPAT_E |           \
              MPAT_RMPID(7) | MPAT_WMPID(7) })

/* Only SPIDs in MPID6 can read, write or execute this region */
#define PTE_VM_FLAGS                                                                  \
    ((mem_flags_t){ .raw = MPAT_UR | MPAT_UW | MPAT_UX | MPAT_SR | MPAT_SW | MPAT_SX | \
              MPAT_E | MPAT_RMPID(6) | MPAT_WMPID(6) })

/* Only SPIDs in MPID6 can read and write this region */
#define PTE_VM_DEV_FLAGS                                                              \
    ((mem_flags_t){ .raw = MPAT_UR | MPAT_UW | MPAT_SR | MPAT_SW | MPAT_E |           \
              MPAT_RMPID(6) | MPAT_WMPID(6) })

static inline size_t mpu_granularity(void)
{
    return (size_t)PAGE_SIZE;
}

#endif /* __ARCH_MEM_H__ */
