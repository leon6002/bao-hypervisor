/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __ARCH_SPINLOCK__
#define __ARCH_SPINLOCK__

#include <bao.h>
#include <util.h>

#define SPINLOCK_INITVAL 0

typedef uint32_t spinlock_t;

static inline void spinlock_init(spinlock_t* lock)
{
    *lock = 0;
}

#if defined(GENERATING_DEFS) || defined(USING_GCC)

static inline void spin_lock(spinlock_t* lock)
{
    /* empty implementation for implicit gcc compilation */
}

static inline void spin_unlock(spinlock_t* lock)
{
    /* empty implementation for implicit gcc compilation */
}

#else

#pragma inline_asm spin_lock
static void spin_lock(spinlock_t* lock)
{
    .LOCAL   _Lock
    .LOCAL   _Lock_wait
    .LOCAL   _Lock_success

      _Lock:
        ldl.w[r6], r19
        cmp r0, r19
        bnz   _Lock_wait
        mov 1, r19
        stc.W r19, [r6]
        cmp r0, r19
        bnz   _Lock_success
      _Lock_wait:
        SNOOZE
        br   _Lock
      _Lock_success:
}

#pragma inline_asm spin_unlock
static void spin_unlock(spinlock_t* lock)
{
    st.w r0, 0 [r6]
}

#endif
#endif /* __ARCH_SPINLOCK__ */
