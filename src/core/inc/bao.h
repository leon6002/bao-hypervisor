/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __BAO_H__
#define __BAO_H__

#include <arch/bao.h>

#ifndef __ASSEMBLER__

/* CC-RH strips the quotes from a -D value, so the version arrives as bare tokens and has to be
 * stringified here. Every other toolchain gets it already quoted from the command line. */
#ifdef CC_IS_RHCC
#define BAO_VERSION_STR_(x) #x
#define BAO_VERSION_STR(x)  BAO_VERSION_STR_(x)
#define BAO_VERSION_STRING  BAO_VERSION_STR(BAO_VERSION)
#else
#define BAO_VERSION_STRING  BAO_VERSION
#endif

#include <types.h>
#include <console.h>
#include <util.h>

#define INFO(...)    console_printk("BAO INFO: " __VA_ARGS__);

#define WARNING(...) console_printk("BAO WARNING: " __VA_ARGS__);

#define ERROR(...)                             \
    console_printk("BAO ERROR: " __VA_ARGS__); \
    while (true) { };

void init(cpuid_t cpu_id);

#endif /* __ASSEMBLER__ */

#endif /* __BAO_H__ */
