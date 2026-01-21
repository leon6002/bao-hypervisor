/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __ARCH_SPINLOCK__
#define __ARCH_SPINLOCK__

#include <bao.h>
#include <util.h>

typedef uint32_t spinlock_t;

#define SPINLOCK_INITVAL 0

static inline void spinlock_init(spinlock_t* lock)
{
    *lock = 0;
}

static inline void spin_lock(spinlock_t* lock)
{
    UNUSED_ARG(lock);
    /* uint32_t const INCR = 1; */
    /* uint32_t ticket; */
    /* uint32_t serving; */

    /* __asm__ volatile( */
    /*     /1* Increment next ticket *1/ */
    /*     "amoadd.w.aqrl  %0, %3, %2 \n\t" */
    /*     "1:\n\t" */
    /*     "lw %1, %4 \n\t" */
    /*     /1* Acquire barrier *1/ */
    /*     "fence r , rw \n\t" */
    /*     /1* Spin on lock if not serving *1/ */
    /*     "bne  %0, %1, 1b \n\t" : "=&r"(ticket), "=&r"(serving), "+A"(lock->next) */
    /*     : "r"(INCR), "A"(lock->ticket) : "memory"); */
}

static inline void spin_unlock(spinlock_t* lock)
{
    /* uint32_t update_lock = lock->ticket + 1; */
    /* __asm__ volatile("fence rw, rw\n\t" */
    /*                  "sw %1, %0 \n\t" : "=A"(lock->ticket) : "r"(update_lock) : "memory"); */
}

#endif /* __ARCH_SPINLOCK__ */
