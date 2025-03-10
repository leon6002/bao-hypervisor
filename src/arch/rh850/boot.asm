;
; SPDX-License-Identifier: Apache-2.0
; Copyright (c) Bao Project and Contributors. All rights reserved.
;

; TODO #include <arch/bao.h>
; #include <asm_defs.h>
; #include <config_defs.h>
; #include <platform_defs.h>


.section ".data", data
.align 2


.extern __s_data_R

.public __load_addr
__load_addr:
    .db2 0x10

.public __data_addr
__data_addr:
    .db2 0x10

.section	".text", text
.align	2
    .public __start
__start:
    di ; Disable interrupts
    ; Disable faults ?

    stsr 0, r5, 2 ; get PEID (regID 0, selID 2)

    mov #_hyp_vector_table, r2
    ldsr r2, 2, 1 ; set RBASE (regID 2, selID 1) ; TODO figure out what this is exactly

    mov #_hyp_interrupt_table, r2
    ldsr r2, 4, 1 ; set INTBP (regID 4, selID 1)

    ; identify master cpu
    ; TODO is this not done already on the platform description?

    ; disable memory protections
    mov r0, r2 ; MPM.MPE i(and all else) disabled
    ldsr r2, 0, 5 ; set MPM

    ;; cover all memory with protection check
    ldsr r0, 8, 5 ; Set the address to MCA
    ldsr r0, 9, 5 ; Set the size to MCS

    ;; individual protections per cpu TODO should it be otherwise?
    ldsr r5, 12, 5 ; set MCI (one spmid per cpu)

    ;; initialize mpu entries for bank 0 TODO use other banks?
    ldsr r0, 17, 5 ; set MPBK
    jarl clear_mpu, lp

    ; enable faults ?

    ; copy non .text segments to ram
    mov #__s.data, r7
    mov #__e.ipi_cpumsg_handlers.const, r6 ; need to copy until
    mov r7, r12
    sub r6, r12 ; r12 is size of data
    mov r12, r13 ; store for later

    mov 0xff000000, r11 ; TODO hopefully use the linker otherwise macro
    ;; copy from [r7] to [r11] r12 bytes
    jarl copy_data, lp

    ; clear .bss
    ;; .bss size
    mov #__s.bss, r12
    mov #__e.ipi_cpumsg_handlers_id.bss, r11
    sub r11, r12; r12 is .bss size

    ;; .bss start
    mov 0xff000000, r11 ; 0xff000000 is begining of data.R
    add r13, r11 ; size of data + beginging of data.R, to get begining of .bss
    mov r11, r14 ; store for later

    ;; .bss end
    add r11, r12 ; r12 is end of .bss
    mov r12, r20 ; store for later
    add 8, r12 ; TODO make sure we cover all .bss memory even we write zero a little bit after

    ;; clear from [r11] to [r12]
    jarl boot_clear, lp

    ; set up this cpus CPU Struct
    ;; r20 contains end of .bss / beginning of struct cpu[]
    ;; CPUx physical based address
    mov 3768, r14 ; CPU_SIZE
    mov r5, r6 ; copy cpu_id to r6
    mulh r14, r6 ; r6 is cpu struct offset
    add r6, r20 ; end of .bss + cpu struct offset

    ;; clear CPUx struct
    add r14, r11
    mov r20, r11
    ;; clear from [r11] to [r12]
    jarl boot_clear, lp

    ldl.w [r6], r21

    ldsr r20, 3, 1 ; use EBASE as CPU* pointer holder

    ; Initialize stack pointer
    mov 3768, r4 ; CPU_STACK_OFF TODO value from .h
    add r10, r4 
    mov 4096, r5 ; CPU STACK SIZE TODO can it be smaller?
    add r5, r4
    mov r4, sp ; set sp
    mov #__start, r5 ; set Text Pointer

    br _init

boot_clear:
boot_clear_2:
    mov 0, r8
    cmp r11, r12
    bge boot_clear_1
    st.w r8, 0[r11]     ; Store 0 to clear memory
    addi 4, r11, r11
    br boot_clear_2
boot_clear_1:
    jmp [lp]

copy_data:
copy_data_1:
    ld.w 0[r7], r8
    st.w r8, 0[r11]
    addi 4, r7, r7
    addi 4, r11, r11
    cmp r11, r12
    bne copy_data_1
    jmp [lp]

clear_mpu:
    mov r0, r5

clear_mpu_1:
    ldsr r5, 16, 5 ; set MPIDX

    ldsr r0, 20, 5 ; set MPLA
    ldsr r0, 21, 5 ; set MPUA
    ldsr r0, 22, 5 ; set MPAT

    cmp 32, r5  ; TODO # of mpu entries platform defined
    addi 1, r5, r5
    bne clear_mpu_1
    jmp [lp]

