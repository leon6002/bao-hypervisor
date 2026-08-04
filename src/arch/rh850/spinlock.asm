;
; SPDX-License-Identifier: Apache-2.0
; Copyright (c) Bao Project and Contributors. All rights reserved.
;
; Spin lock for the CC-RH build.
;
; This is a real assembly function rather than a #pragma inline_asm one because the lock loop
; needs labels, and inline_asm bodies are expanded at every call site, which would emit the
; same label more than once (User's Manual 4.2.6.2). A single out-of-line definition avoids
; that entirely, at the cost of a call.
;
; r6 holds the lock pointer per the CC-RH calling convention. r19 is a work register, free to
; clobber without saving; r20 upwards are callee-save and must not be touched here.

    .section ".text", text

    .public _spin_lock
_spin_lock:
_spin_lock_retry:
    ldl.w   [r6], r19           ; load-linked, r19 = *lock
    cmp     r0, r19
    bnz     _spin_lock_wait     ; already held, go spin
    mov     1, r19
    stc.w   r19, [r6]           ; store-conditional; r19 = 1 on success
    cmp     r0, r19
    bnz     _spin_lock_done     ; took it
_spin_lock_wait:
    snooze
    br      _spin_lock_retry
_spin_lock_done:
    jmp     [lp]

    .public _spin_unlock
_spin_unlock:
    st.w    r0, 0[r6]           ; release
    jmp     [lp]
