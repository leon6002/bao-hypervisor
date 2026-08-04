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

/*
 * CC-RH accepts __attribute__((aligned(n))) on a member only up to 4 bytes, and not at all on a
 * struct, so PAGE_SIZE alignment cannot be expressed in C on that toolchain.
 *
 * TODO: the CC-RH build currently drops the alignment. It is not needed to find the current
 * cpu -- cpu() reads the pointer out of FEWR -- but the MPU maps these structures and its
 * regions must start on a granularity boundary, so this has to be given back through the
 * linker (a dedicated section plus rlink section alignment) before the build is trusted.
 */
#ifdef CC_IS_RHCC
#define ATTRIB_ALIGN(n)
#else
#define ATTRIB_ALIGN(n) __attribute__((aligned(n)))
#endif

#ifdef CC_IS_RHCC
/* CC-RH has no fallthrough attribute; falling through is simply not diagnosed there. */
#define FALLTHROUGH
#else
#define FALLTHROUGH __attribute__((fallthrough))
#endif

#ifdef CC_IS_RHCC
/* CC-RH compiles as C99 and has no _Alignof. This is the usual C99 stand-in for it. */
#include <stddef.h>
#define _Alignof(type)                \
    offsetof(struct {                 \
        char _alignof_c;              \
        type _alignof_m;              \
    },                                \
        _alignof_m)
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
