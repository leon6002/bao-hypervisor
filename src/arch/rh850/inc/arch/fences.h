/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __ARCH_FENCES_H__
#define __ARCH_FENCES_H__

#include <bao.h>

#ifdef CC_IS_RHCC

/* CC-RH has no GNU extended asm; #pragma inline_asm is the equivalent. Each body is a single
 * instruction with no operands and no labels, so it expands safely at every call site. */
#pragma inline_asm syncp
static void syncp(void)
{
    syncp
}

#pragma inline_asm syncm
static void syncm(void)
{
    syncm
}

#pragma inline_asm synci
static void synci(void)
{
    synci
}

#pragma inline_asm synce
static void synce(void)
{
    synce
}

#else

static inline void syncp(void)
{
    __asm__ volatile("syncp" ::: "memory");
}

static inline void syncm(void)
{
    __asm__ volatile("syncm" ::: "memory");
}

static inline void synci(void)
{
    __asm__ volatile("synci" ::: "memory");
}

static inline void synce(void)
{
    __asm__ volatile("synce" ::: "memory");
}

#endif

static inline void fence_ord_write(void)
{
    syncm();
}

static inline void fence_ord_read(void)
{
    syncp();
}

static inline void fence_ord(void)
{
    syncm();
}

static inline void fence_sync_write(void)
{
    syncm();
}

static inline void fence_sync_read(void)
{
    syncp();
}

static inline void fence_sync(void)
{
    syncm();
}

#endif /* __ARCH_FENCES_H__ */
