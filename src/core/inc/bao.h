/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __BAO_H__
#define __BAO_H__

#include <arch/bao.h>

#ifndef __ASSEMBLER__

#include <types.h>
#include <console.h>
#include <util.h>

#define INFO(...)    console_printk("BAO INFO: " __VA_ARGS__);

#define WARNING(...) console_printk("BAO WARNING: " __VA_ARGS__);

#define ERROR(...)                             \
    console_printk("BAO ERROR: " __VA_ARGS__); \
    while (true) { };

void init(cpuid_t cpu_id);

#ifdef CC_IS_RHCC

#define ATTRIBUTE_ALIGN(x) __align((x))
#define FALLTHROUGH        // FALLTHROUGH

#else

#define ATTRIBUTE_ALIGN(x) __attribute__((aligned((x))))
#define FALLTHROUGH        __attribute__((fallthrough))

#endif

#endif /* __ASSEMBLER__ */

#endif /* __BAO_H__ */
