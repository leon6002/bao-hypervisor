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
.extern _CPU_MASTER

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
    ldsr r2, 2, 1 ; set RBASE (regID 2, selID 1)

    mov #_hyp_interrupt_table, r2
    ldsr r2, 4, 1 ; set INTBP (regID 4, selID 1)

    ; identify master cpu
    mov r0, r10 ; get value from CPU_MASTER_FIXED
    mov #_CPU_MASTER, r20
    st.w r10, 0[r20]

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

    ; check if current CPU is CPU_MASTER
    cmp r5, r10
    bne clear_cpu

    ;--------------------- Assuming execution from FLASH ---------------------;
    ; ; copy non .text segments to ram
    ; mov #__s.data, r20 ; start of the region
    ; mov #__e.ipi_cpumsg_handlers.const, r21 ; end of the region
    ; mov r21, r22 ; keep the end of the region in r21
    ; sub r20, r22 ; r22 is size of the region
    ; mov r22, r6 ; store data size for later

    ; mov 0xff000000, r23 ; TODO hopefully use the linker otherwise macro
    ; ;; copy from [r20] to [r23] r22 bytes
    ; jarl copy_data, lp

    ; ; clear .bss
    ; ;; .bss size
    ; mov #__s.bss, r20
    ; mov #__e.ipi_cpumsg_handlers_id.bss, r21
    ; sub r20, r21; r21 is .bss size

    ; ;; .bss start
    ; mov 0xff000000, r20 ; 0xff000000 is begining of data.R
    ; add r6, r20 ; size of data + beginging of data.R = begining of .bss
    ; mov r20, r7 ; store begining of .bss for later

    ; ;; .bss end
    ; add r20, r21 ; r21 becomes end of .bss
    ; mov r21, r8 ; store end of .bss for later
    ; add 8, r21 ; TODO make sure we cover all .bss memory even we write zero a little bit after

    ;---------------------- Assuming execution from RAM ----------------------;
    mov #__s.bss, r20
    mov #__e.ipi_cpumsg_handlers_id.bss, r21

    ;; clear from [r20] to [r21]
    jarl boot_clear, lp

    ; Set r8 with the end of .bss
    mov #__e.ipi_cpumsg_handlers_id.bss, r8

    ;-------------------------------------------------------------------------;

clear_cpu:
    ; set up this cpu's CPU Struct
    ;; r8 contains end of .bss / beginning of struct cpu
    ;; CPUx physical based address
    mov 7352, r9 ; CPU_SIZE TODO value from .h
    mov r5, r20 ; copy cpu_id to r20
    mulh r9, r20 ; r20 is cpu struct offset
    add r20, r8 ; end of .bss + cpu struct offset = r8 points to cpu

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

    mov #_init, r5 ; set Text Pointer TODO what should be here?
    br _init

boot_clear:
boot_clear_1:
    mov 0, r22
    cmp r21, r20
    bge boot_clear_exit
    st.w r22, 0[r20]     ; Store 0 to clear memory
    addi 4, r20, r20
    br boot_clear_1
boot_clear_exit:
    jmp [lp]

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

