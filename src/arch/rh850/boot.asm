;
; SPDX-License-Identifier: Apache-2.0
; Copyright (c) Bao Project and Contributors. All rights reserved.
;

; TODO #include <arch/bao.h>
; #include <asm_defs.h>
; #include <config_defs.h>
; #include <platform_defs.h>


.section ".data", data
.align 4

.extern __s_data_R
.extern _CPU_MASTER

; Global variables
.public __load_addr
__load_addr:
    .db4 0x0

.public __data_addr
__data_addr:
    .db4 0xFE000000

; Aren't these already declared in core/mem.c ?
.public __image_start
.public __image_load_end
.public __image_noload_start
.public __image_end
;--------------------------------------------------

.public __S_text

.public __E_bss_R
.public __S_bss_R

.public __S_ipi_cpumsg_handlers_const_R

.public __E_ipi_cpumsg_handlers_id_data_R
.public __S_ipi_cpumsg_handlers_id_data_R

.public __S_data_R

.section ".text", text
.align	2
    .public __start
__start:
    di ; Disable interrupts
    ; Disable faults ?

    ; get current CPU
    stsr 0, r5, 2 ; get PEID (regID 0, selID 2)
    ; identify master cpu
    mov r0, r10 ; TODO: get value from CPU_MASTER_FIXED

    mov #_hyp_vector_table, r2
    ldsr r2, 2, 1 ; set RBASE (regID 2, selID 1)

    mov #_hyp_interrupt_table, r2
    ldsr r2, 4, 1 ; set INTBP (regID 4, selID 1)

    ; disable memory protections
    mov r0, r2 ; MPM.MPE (and all else) disabled
    ldsr r2, 0, 5 ; set MPM

    ;; cover all memory with protection check
    ldsr r0, 8, 5 ; Set the address to MCA
    ldsr r0, 9, 5 ; Set the size to MCS

    ;; individual protections per cpu TODO should it be otherwise?
    ldsr r0, 12, 5 ; set MCI to 0
    ldsr r0, 0, 1 ; set SPID host SPID 0

    ;; initialize mpu entries for bank 0 TODO use other banks?
    ldsr r0, 17, 5 ; set MPBK
    jarl clear_mpu, lp

    ; enable faults ?

    mov #__s.text, r20
    mov #__S_text, r21
    st.w r20, 0[r21]

    ; save limits from non .text sections
    ;; save .data start
    mov #__S_data_R, r20
    mov 0xfe000000, r21
    st.w r21, 0[r20]

    ;; calculate start of .ipi_cpumsg_handlers_id.data.R
    mov #__s.data, r20
    mov #__e.data, r21
    sub r20, r21 ; r21 holds .data size
    mov 0xfe000000, r20
    add r20, r21 ; r21 is the end of .data.R = start of .ipi_cpumsg_handlers_id.data.R
    mov r21, r22 ; save for next section
    mov #__S_ipi_cpumsg_handlers_id_data_R, r20
    st.w r21, 0[r20]

    ;; calculate end of .ipi_cpumsg_handlers_id.data.R
    mov #__s.ipi_cpumsg_handlers_id.data, r20
    mov #__e.ipi_cpumsg_handlers_id.data, r21
    sub r20, r21 ; r21 holds .ipi_cpumsg_handlers_id.data size
    add r22, r21 ; r21 is the end of .ipi_cpumsg_handlers_id.data.R
    mov r21, r22 ; save for next section
    mov #__E_ipi_cpumsg_handlers_id_data_R, r20
    st.w r21, 0[r20]
    mov #__S_ipi_cpumsg_handlers_const_R, r20
    st.w r21, 0[r20]

    ; copy non .text segments to ram
    mov #__s.data, r20
    mov #__e.ipi_cpumsg_handlers.const, r21 ; need to copy until
    mov r21, r22
    sub r20, r22 ; r22 is size of data
    mov r22, r6 ; store for later

    ; check if current CPU is CPU_MASTER
    cmp r5, r10
    bne skip_copy_data

    mov 0xfe000000, r23 ; TODO hopefully use the linker otherwise macro
    ;; copy from [r20] to [r23] r22 bytes
    jarl copy_data, lp

skip_copy_data:
    ; clear .bss
    ;; .bss size
    mov #__s.bss, r20
    mov #__e.bss, r21
    sub r20, r21; r21 is .bss size

    ;; .bss start
    mov 0xfe000000, r20 ; 0xfe000000 is begining of data.R
    add r6, r20 ; size of data + beginging of data.R = begining of .bss.R
    mov r20, r7 ; store for later
    mov #__S_bss_R, r22
    st.w r20, 0[r22]

    ;; .bss end
    add r20, r21 ; r21 becomes end of .bss
    mov r21, r8 ; store for later
    mov #__S_bss_R, r22
    st.w r21, 0[r22]
 
    ; check if current CPU is CPU_MASTER
    cmp r5, r10
    bne clear_cpu

    ;; clear from [r20] to [r21]
    jarl boot_clear, lp

    mov #_CPU_MASTER, r20
    st.w r10, 0[r20]

clear_cpu:
    ; set up this cpu's CPU Struct
    ;; r8 contains end of .bss / beginning of struct cpu
    ;; CPUx physical based address
    mov 7352, r9 ; CPU_SIZE TODO value from .h
    mov r5, r20 ; copy cpu_id to r20
    mulh r9, r20 ; r20 is cpu struct offset
    add r20, r8 ; end of .bss + cpu struct offset = r8 points to cpu
    
    ;; align CPU pointer to 64 bytes
    ;; TODO: get granularity from .h
    addi 64, r8, r8
    mov 0x3F, r20
    not r20, r20
    and r20, r8

    ;; clear CPUx struct
    mov r8, r20
    mov r8, r21
    add r9, r21
    ;; clear from [r20] to [r21]
    jarl boot_clear, lp

    ldsr r8, 29, 0 ; use FEWR as CPU* pointer holder

    ; Initialize stack pointer
    mov 3256, r20 ; CPU_STACK_OFF TODO value from .h
    add r8, r20 ; add stack offset to CPU pointer
    mov 4096, r21 ; CPU STACK SIZE TODO can it be smaller?
    add r21, r20
    mov r20, sp ; set sp

    ; Set init arguments
    mov r5, r6 ; copy CPU ID to r6
    mov #_init, r5 ; set Text Pointer TODO what should be here?    
    br _init

; r20: start of region
; r21: end of region
boot_clear:
boot_clear_1:
    cmp r21, r20
    bge boot_clear_exit
    st.w r0, 0[r20]     ; Store 0 to clear memory
    addi 4, r20, r20
    br boot_clear_1
boot_clear_exit:
    jmp [lp]


; r20: start of the source region
; r21: end of the source region
; r22: size of the region
; r23: start of the destination region
copy_data:
copy_data_1:
    ld.w 0[r20], r24
    st.w r24, 0[r23]
    addi 4, r20, r20
    addi 4, r23, r23
    cmp r20, r21
    bne copy_data_1
    jmp [lp]


clear_mpu:
    mov r0, r20
clear_mpu_1:
    cmp 32, r20  ; TODO # of mpu entries platform defined
    be clear_mpu_exit

    ldsr r20, 16, 5 ; set MPIDX

    ldsr r0, 20, 5 ; set MPLA
    ldsr r0, 21, 5 ; set MPUA
    ldsr r0, 22, 5 ; set MPAT

    addi 1, r20, r20
    bne clear_mpu_1
clear_mpu_exit:
    jmp [lp]
