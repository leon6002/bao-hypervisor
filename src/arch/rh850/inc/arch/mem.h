/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __ARCH_MEM_H__
#define __ARCH_MEM_H__

#include <bao.h>

#define HYP_SPID 0x0
#define VM_SPID 0x1

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
    } b;
    uint32_t raw;
} mpat_flags_t;

typedef mpat_flags_t mem_flags_t;

/**
 * The permission constants below are composed through .raw rather than through the bit field.
 * CC-RH rejects designated initializers for members of an anonymous struct or union (E0523134),
 * and initializing the named field (.b) instead makes it emit a field-by-field store sequence at
 * every use site rather than folding the value into a single word (+2.3 KiB of .text).
 *
 * These mirror the bit field above; keep both in sync.
 */
#define MPAT_UR             (1UL << 0)
#define MPAT_UW             (1UL << 1)
#define MPAT_UX             (1UL << 2)
#define MPAT_SR             (1UL << 3)
#define MPAT_SW             (1UL << 4)
#define MPAT_SX             (1UL << 5)
#define MPAT_E              (1UL << 7)
#define MPAT_RG             (1UL << 14)
#define MPAT_WG             (1UL << 15)
#define MPAT_RMPID(n)       (1UL << (16 + (n)))
#define MPAT_WMPID(n)       (1UL << (24 + (n)))

#define PTE_INVALID         ((mem_flags_t){ .raw = 0 })

#define PTE_HYP_CODE_FLAGS  ((mem_flags_t){ .raw = MPAT_E | MPAT_SR | MPAT_SX | MPAT_RG })
#define PTE_HYP_FLAGS                                                            \
    ((mem_flags_t){ .raw = MPAT_E | MPAT_SR | MPAT_SW | MPAT_SX | MPAT_RMPID(0) | \
              MPAT_WMPID(0) })
#define PTE_HYP_DEV_FLAGS                                                        \
    ((mem_flags_t){ .raw = MPAT_E | MPAT_SR | MPAT_SW | MPAT_RMPID(0) | MPAT_WMPID(0) })

/* TODO in the future we need to deal with IO permissions securely */
#define PTE_VM_FLAGS                                                             \
    ((mem_flags_t){ .raw = MPAT_E | MPAT_SR | MPAT_SW | MPAT_SX | MPAT_UR | MPAT_UW | \
              MPAT_UX | MPAT_RMPID(1) | MPAT_WMPID(1) })

/* TODO in the future we need to deal with IO permissions securely */
#define PTE_VM_DEV_FLAGS                                                         \
    ((mem_flags_t){ .raw = MPAT_E | MPAT_SR | MPAT_SW | MPAT_UR | MPAT_UW | MPAT_RMPID(1) | \
              MPAT_WMPID(1) })

#define MPU_ARCH_MAX_NUM_ENTRIES (32)

static inline size_t mpu_granularity(void)
{
    return (size_t)PAGE_SIZE;
}

#endif /* __ARCH_MEM_H__ */
