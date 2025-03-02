/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <cache.h>
#include <arch/sysregs.h>
#include <fences.h>
#include <bit.h>
#include <platform.h>

void cache_arch_enumerate(struct cache* dscrp) { }

void cache_flush_range(vaddr_t base, size_t size) { }
