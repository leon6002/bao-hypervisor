/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __ARCH_SPINLOCK__
#define __ARCH_SPINLOCK__

#include <arch/spinlock.h>

#pragma inline_asm spin_lock
void spin_lock(spinlock_t* lock)
{
Lock:
    ldl.w [r6], r21
    cmp r0, r21
    bnz Lock_wait
    mov 1, r21
    stc.W r21, [r6]
    cmp r0, r21
    bnz Lock_success
Lock_wait: SNOOZE
    br Lock
    Lock_success:
}

#pragma inline_asm spin_unlock
void spin_unlock(spinlock_t* lock)
{
    st.w r0, 0[r6]
}


#endif /* __ARCH_SPINLOCK__ */
