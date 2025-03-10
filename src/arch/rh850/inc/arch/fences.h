/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __FENCES_ARCH_H__
#define __FENCES_ARCH_H__

#include <bao.h>

#pragma inline_asm syncp
static unsigned long syncp(void)
{
    syncp
}

#pragma inline_asm syncm
static unsigned long syncm(void)
{
    syncm
}

#pragma inline_asm synci
static unsigned long synci(void)
{
    synci
}

#pragma inline_asm synce
static unsigned long synce(void)
{
    synce
}

static inline void fence_ord_write(void)
{
    synci();
}

static inline void fence_ord_read(void)
{
    synci();
}

static inline void fence_ord(void)
{
    synci();
}

static inline void fence_sync_write(void)
{
    synci();
}

static inline void fence_sync_read(void)
{
    synci();
}

static inline void fence_sync(void)
{
    synci();
}

#endif /* __FENCES_ARCH_H__ */
