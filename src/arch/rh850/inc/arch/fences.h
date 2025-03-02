/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __FENCES_ARCH_H__
#define __FENCES_ARCH_H__

#include <bao.h>

static inline void fence_ord_write(void) { }

static inline void fence_ord_read(void) { }

static inline void fence_ord(void) { }

static inline void fence_sync_write(void) { }

static inline void fence_sync_read(void) { }

static inline void fence_sync(void) { }

#endif /* __FENCES_ARCH_H__ */
