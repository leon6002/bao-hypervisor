;
; SPDX-License-Identifier: Apache-2.0
; Copyright (c) Bao Project and Contributors. All rights reserved.
;
; asrh version of exceptions.S.
;
; The GNU original keeps the context save and restore in .macro definitions. asrh has macros
; too, but a label inside one is emitted verbatim at every expansion and the assembler then
; rejects the redefinition, and its parameter syntax differs from GNU's. Since there are only
; two expansions of each, they are written out at their use sites with labels named after the
; site. Verbose, but there is nothing to get wrong in it.
;
; asrh does not run the preprocessor, so the build expands this with the host cpp first.

#include <bao.h>
#include <asm_defs.h>
#include <arch/vmm.h>

    .section ".text", text

; ---------------------------------------------------------------------------------------------
; Exception vector table. The reset entry is never taken here -- boot.asm holds the entry
; point -- so it parks the core.
; ---------------------------------------------------------------------------------------------
    .align 512
    .public _hyp_vector_table
_hyp_vector_table:
.L_reset_park:
    br    .L_reset_park         ; RESET

    .align 16
    br    _guest_exception      ; SYSERR
    .align 16
    br    _guest_exception      ; HVTRAP
    .align 16
    br    _guest_exception      ; FETRAP
    .align 16
    br    _guest_exception      ; TRAP0
    .align 16
    br    _guest_exception      ; TRAP1
    .align 16
    br    _host_exception       ; RIE
    .align 16
    br    _host_exception       ; FPE/FXE
    .align 16
    br    _host_exception       ; UCPOP
    .align 16
    br    _guest_exception      ; MIP/MDP
    .align 16
    br    _host_exception       ; PIE
    .align 16
    br    _host_exception       ; R.F.U.
    .align 16
    br    _host_exception       ; MAE
    .align 16
    br    _host_exception       ; BGFEINT/BGEIINT
    .align 16
    br    _host_exception       ; FENMI
    .align 16
    br    _host_exception       ; FEINT/GMFEINT

    ; direct vector method only
    .align 16
    br    _Interrupt_EI         ; INTn priority 0
    .align 16
    br    _Interrupt_EI         ; INTn priority 1
    .align 16
    br    _Interrupt_EI         ; INTn priority 2
    .align 16
    br    _Interrupt_EI         ; INTn priority 3
    .align 16
    br    _Interrupt_EI         ; INTn priority 4
    .align 16
    br    _Interrupt_EI         ; INTn priority 5
    .align 16
    br    _Interrupt_EI         ; INTn priority 6
    .align 16
    br    _Interrupt_EI         ; INTn priority 7
    .align 16
    br    _Interrupt_EI         ; INTn priority 8
    .align 16
    br    _Interrupt_EI         ; INTn priority 9
    .align 16
    br    _Interrupt_EI         ; INTn priority 10
    .align 16
    br    _Interrupt_EI         ; INTn priority 11
    .align 16
    br    _Interrupt_EI         ; INTn priority 12
    .align 16
    br    _Interrupt_EI         ; INTn priority 13
    .align 16
    br    _Interrupt_EI         ; INTn priority 14
    .align 16
    br    _Interrupt_EI         ; INTn priority 15

; ---------------------------------------------------------------------------------------------
_guest_exception:
    ; ---- VM_EXIT ----
    ldsr r31, 28, 0                 ; EIWR as scratchpad

    stsr 29, r31, 0                 ; cpu* from FEWR
    ld.w CPU_VCPU_OFF[r31], r31     ; vcpu*
    add VCPU_REGS_OFF, r31
    st.w  r1,  4[r31]
    st.dw r2,  8[r31]
    st.dw r4,  16[r31]
    st.dw r6,  24[r31]
    st.dw r8,  32[r31]
    st.dw r10, 40[r31]
    st.dw r12, 48[r31]
    st.dw r14, 56[r31]
    st.dw r16, 64[r31]
    st.dw r18, 72[r31]
    st.dw r20, 80[r31]
    st.dw r22, 88[r31]
    st.dw r24, 96[r31]
    st.dw r26, 104[r31]
    st.dw r28, 112[r31]
    st.w  r30, 120[r31]

    mov r31, r30
    stsr 28, r31, 0
    st.w r31, 124[r30]

    stsr 5, r20, 0                  ; PC comes from FEPC or EIPC
    andi 0x80, r20, r20
    cmp r0, r20
    be .L_ge_eipc
    stsr 2, r20, 0                  ; FEPC
    br .L_ge_pc_done
.L_ge_eipc:
    stsr 0, r20, 0                  ; EIPC
.L_ge_pc_done:
    st.w r20, 128[r30]

    stsr 29, r20, 0                 ; back onto the hypervisor stack
    mov CPU_STACK_OFF, r21
    add r21, r20
    mov CPU_STACK_SIZE, r21
    add r21, r20
    mov r20, sp
    ; ---- end VM_EXIT ----

    jarl _abort, lp

    ; ---- VM_ENTRY ----
    stsr 29, r30, 0
    ld.w CPU_VCPU_OFF[r30], r30
    add VCPU_REGS_OFF, r30

    ld.w 128[r30], r20              ; PC goes back to FEPC or EIPC
    stsr 5, r21, 0
    andi 0x80, r21, r21
    cmp r0, r21
    be .L_ge_ret_eipc
    ldsr r20, 2, 0                  ; FEPC
    br .L_ge_ret_done
.L_ge_ret_eipc:
    ldsr r20, 2, 0                  ; EIPC
.L_ge_ret_done:
    mov r30, r31

    ld.w  4[r31], r1
    ld.dw 8[r31], r2
    ld.dw 16[r31], r4
    ld.dw 24[r31], r6
    ld.dw 32[r31], r8
    ld.dw 40[r31], r10
    ld.dw 48[r31], r12
    ld.dw 56[r31], r14
    ld.dw 64[r31], r16
    ld.dw 72[r31], r18
    ld.dw 80[r31], r20
    ld.dw 88[r31], r22
    ld.dw 96[r31], r24
    ld.dw 104[r31], r26
    ld.dw 112[r31], r28
    ld.w  120[r31], r30

    ld.w 124[r31], r31              ; EIWR as scratchpad
    ldsr r31, 28, 0

    stsr 5, r31, 0                  ; eiret or feret, restoring r31 from EIWR
    andi 0x80, r31, r31
    cmp r0, r31
    be .L_ge_eiret
    stsr 28, r31, 0
    feret
.L_ge_eiret:
    stsr 28, r31, 0
    eiret
    ; ---- end VM_ENTRY ----

; ---------------------------------------------------------------------------------------------
_host_exception:
    jarl _internal_abort, lp

; ---------------------------------------------------------------------------------------------
_Interrupt_EI:
    ; ---- VM_EXIT ----
    ldsr r31, 28, 0

    stsr 29, r31, 0
    ld.w CPU_VCPU_OFF[r31], r31
    add VCPU_REGS_OFF, r31
    st.w  r1,  4[r31]
    st.dw r2,  8[r31]
    st.dw r4,  16[r31]
    st.dw r6,  24[r31]
    st.dw r8,  32[r31]
    st.dw r10, 40[r31]
    st.dw r12, 48[r31]
    st.dw r14, 56[r31]
    st.dw r16, 64[r31]
    st.dw r18, 72[r31]
    st.dw r20, 80[r31]
    st.dw r22, 88[r31]
    st.dw r24, 96[r31]
    st.dw r26, 104[r31]
    st.dw r28, 112[r31]
    st.w  r30, 120[r31]

    mov r31, r30
    stsr 28, r31, 0
    st.w r31, 124[r30]

    stsr 5, r20, 0
    andi 0x80, r20, r20
    cmp r0, r20
    be .L_ei_eipc
    stsr 2, r20, 0                  ; FEPC
    br .L_ei_pc_done
.L_ei_eipc:
    stsr 0, r20, 0                  ; EIPC
.L_ei_pc_done:
    st.w r20, 128[r30]

    stsr 29, r20, 0
    mov CPU_STACK_OFF, r21
    add r21, r20
    mov CPU_STACK_SIZE, r21
    add r21, r20
    mov r20, sp
    ; ---- end VM_EXIT ----

    stsr 13, r6, 0                  ; EIIC -> int_id in r6 (TODO: FEIC)
    mov 0x7FF, r20
    and r20, r6

    jarl _interrupts_handle, lp

; ---------------------------------------------------------------------------------------------
    .public _vcpu_arch_entry
_vcpu_arch_entry:
    ; ---- VM_ENTRY ----
    stsr 29, r30, 0
    ld.w CPU_VCPU_OFF[r30], r30
    add VCPU_REGS_OFF, r30

    ld.w 128[r30], r20
    stsr 5, r21, 0
    andi 0x80, r21, r21
    cmp r0, r21
    be .L_va_ret_eipc
    ldsr r20, 2, 0                  ; FEPC
    br .L_va_ret_done
.L_va_ret_eipc:
    ldsr r20, 2, 0                  ; EIPC
.L_va_ret_done:
    mov r30, r31

    ld.w  4[r31], r1
    ld.dw 8[r31], r2
    ld.dw 16[r31], r4
    ld.dw 24[r31], r6
    ld.dw 32[r31], r8
    ld.dw 40[r31], r10
    ld.dw 48[r31], r12
    ld.dw 56[r31], r14
    ld.dw 64[r31], r16
    ld.dw 72[r31], r18
    ld.dw 80[r31], r20
    ld.dw 88[r31], r22
    ld.dw 96[r31], r24
    ld.dw 104[r31], r26
    ld.dw 112[r31], r28
    ld.w  120[r31], r30

    ld.w 124[r31], r31
    ldsr r31, 28, 0

    stsr 5, r31, 0
    andi 0x80, r31, r31
    cmp r0, r31
    be .L_va_eiret
    stsr 28, r31, 0
    feret
.L_va_eiret:
    stsr 28, r31, 0
    eiret
    ; ---- end VM_ENTRY ----
