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
    .db4 0xfe100000

; __sVECTAB
.public __image_start
__image_start:
    .db4 0x0

; __s.bss
.public __image_load_end
__image_load_end:
    .db4 0x0

; __s.bss.R
.public __image_noload_start
__image_noload_start:
    .db4 0x0

; __e.bss.R
.public __image_end
__image_end:
    .db4 0x0

; __s.ipi_cpumsg_handlers.const.R
.public _ipi_cpumsg_handlers
_ipi_cpumsg_handlers:
    .db4 0x0

; __s.ipi_cpumsg_handlers_id.data.R
.public __ipi_cpumsg_handlers_id_start
__ipi_cpumsg_handlers_id_start:
    .db4 0x0

; __e.ipi_cpumsg_handlers_id.data.R
.public __ipi_cpumsg_handlers_id_end
__ipi_cpumsg_handlers_id_end:
    .db4 0x0

; __s.data.R
.public __data_vma_start
__data_vma_start:
    .db4 0x0

.section ".text", text
.align	2
    .public __start
__start:
    di ; Disable interrupts

    ; get current CPU
    stsr 0, r5, 2 ; get PEID (regID 0, selID 2)
    ; identify master cpu
    mov r0, r10 ; TODO: get value from CPU_MASTER_FIXED

_loop_3:
    mov 3, r20
    cmp r5, r20
    be _loop_3
    mov 2, r20
    cmp r5, r20
    be _loop_3
    ; mov 1, r20
    ; cmp r5, r20
    ; be _loop_3

    mov 0x8020, r2
    ldsr r2, 5, 0 ; set PSW.EBV

    mov #_hyp_vector_table, r2
    ori 0x2, r2, r2 ; select direct vector method
    ldsr r2, 3, 1 ; set EBASE (regID 3, selID 1)

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
    jarl _clear_mpu, lp

    ; calculate offset between flash and RAM
    ;; we asume #__s.data < #__s.data.R
    mov 0xfe100000, r20
    mov #__s.data, r21
    sub r21, r20
    mov r20, r6 ; store offset

    ; initialize local RAM
    mov 0xFDE00000, r20
    mov 0xFDE0FFFF, r21
    jarl _ram_init, lp

    ; enable faults ?

    ; check if current CPU is CPU_MASTER
    cmp r5, r10
    ; bne _check_barrier
    bne _clear_cpu

    ; enable interrupt virtualization support
    ;; recommended at CPU initialization after reset
    mov 0xFFFC402F0, r20
    mov 0x1, r21
    st.w r21, 0[r20] ; IHVCFG.IHVE = 0x1

    ; initialize cluster RAM
    mov 0xFE000000, r20
    mov 0xFE07FFFF, r21
    jarl _ram_init, lp

    mov 0xFE100000, r20
    mov 0xFE17FFFF, r21
    jarl _ram_init, lp

    mov 0xFE400000, r20
    mov 0xFE5FFFFF, r21
    jarl _ram_init, lp

    mov 0xFE800000, r20
    mov 0xFE83FFFF, r21
    jarl _ram_init, lp
    
    ; copy non .text segments to ram
    mov #__s.data, r20
    mov #__e.ipi_cpumsg_handlers.const, r21 ; need to copy until
    mov 0xfe100000, r22 ; TODO hopefully use the linker otherwise macro
    ;; copy from [r20] until [r21] to [r22]
    jarl _copy_data, lp

    ; clear .bss
    ;; .bss start
    mov #__s.bss, r20
    add r6, r20 ; add offset
    mov #__image_noload_start, r22
    st.w r20, 0[r22]

    ;; .bss end
    mov #__e.bss, r21
    add r6, r21 ; add offset
    addi 63, r21, r21
    mov 0x3f, r7
    not r7, r7
    and r7,  r21
    mov #__image_end, r22
    st.w r21, 0[r22]

    ;; clear from [r20] to [r21]
    jarl _boot_clear, lp

    ; store CPU_MASTER
    mov #_CPU_MASTER, r20
    st.w r10, 0[r20]

    ; save section boundaries
    ;; sections within flash
    mov #__sVECTAB, r20
    mov #__image_start, r21
    st.w r20, 0[r21]
    mov #__s.bss, r20
    mov #__image_load_end, r21
    st.w r20, 0[r21]

    ;; start of RAM
    mov 0xfe100000, r20
    mov #__data_vma_start, r21
    st.w r20, 0[r21]

    ;; sections within RAM
    mov #__s.ipi_cpumsg_handlers_id.data, r20
    add r6, r20 ; add offset
    mov #__ipi_cpumsg_handlers_id_start, r21
    st.w r20, 0[r21]
    mov #__e.ipi_cpumsg_handlers_id.data, r20
    add r6, r20 ; add offset
    mov #__ipi_cpumsg_handlers_id_end, r21
    st.w r20, 0[r21]
    mov #_ipi_cpumsg_handlers, r21
    st.w r20, 0[r21]

; _check_barrier:
;     ; Initialize CPU barrier
;     ;; Write 0x3 to BR0EN
;     mov 0xFFFB8004, r20
;     mov 0x3, r21 ; TODO: create macro for BR0EN macro
;     st.b r21, 0[r20]
;     ; ;; Write 0x1 to BR0INIT
;     ; mov 0xFFFB8000, r20
;     ; mov 0x1, r21
;     ; st.b r21, 0[r20]
;     ;; Write 0x1 to BR0CHKS
;     mov 0xFFFB8100, r20
;     mov 0x1, r21
;     st.b r21, 0[r20]

; _poll_barrier:
;     ; Poll BR0SYNCS
;     mov 0xFFFB8104, r20
;     ld.bu 0[r20], r21
;     cmp r0, r21
;     be _poll_barrier
    
_clear_cpu:
    ; set up CPUn Struct
    ;; .bss.R end
    mov #__e.bss, r20
    add r6, r20 
    addi 63, r20, r20
    mov 0x3F, r21
    not r21, r21
    and r21, r20 ; r20 holds end of .bss.R aligned to 64 bytes (PAGE_SIZE - TODO: try use macro)
    ;; CPUx physical based address
    mov 0x1840, r7 ; CPU_SIZE TODO value from .h (aligned to PAGE_SIZE)
    mov r5, r21 ; copy cpu_id to r21
    mulh r7, r21 ; r21 is cpu struct offset
    add r21, r20 ; end of .bss + cpu struct offset = r20 points to cpu
    mov r20, r8 ; r8 holds the CPU pointer aligned to PAGE_SIZE

    ;; clear CPUx struct
    mov r20, r21
    add r7, r21
    ;; clear from [r20] to [r21]
    jarl _boot_clear, lp

    ldsr r8, 29, 0 ; use FEWR as CPU* pointer holder

    ; Initialize stack pointer
    mov 0x838, r20 ; CPU_STACK_OFF TODO value from .h
    add r8, r20 ; add stack offset to CPU pointer
    mov 0x1000, r21 ; CPU STACK SIZE TODO can it be smaller?
    add r21, r20
    mov r20, sp ; set sp
    
    ; Set init arguments
    mov r5, r6 ; copy CPU ID to r6
    mov #_init, r5

    br _init

; r20: start of region
; r21: end of region
_ram_init:
    br _ram_init_2
_ram_init_1:
    st.w r0, 0[r20]
    add 4, r20
_ram_init_2:
    cmp r20, r21
    bh _ram_init_1
    jmp [lp]

; r20: start of region
; r21: end of region
_boot_clear:
_boot_clear_1:
    cmp r21, r20
    bge _boot_clear_exit
    st.w r0, 0[r20]     ; Store 0 to clear memory
    addi 4, r20, r20
    br _boot_clear_1
_boot_clear_exit:
    jmp [lp]


; r20: start of the source region
; r21: end of the source region
; r22: start of the destination region
; uses r23
_copy_data:
_copy_data_1:
    ld.w 0[r20], r23
    st.w r23, 0[r22]
    addi 4, r20, r20
    addi 4, r22, r22
    cmp r20, r21
    bne _copy_data_1
    jmp [lp]


_clear_mpu:
    mov r0, r20
_clear_mpu_1:
    cmp 32, r20  ; TODO # of mpu entries platform defined
    be _clear_mpu_exit

    ldsr r20, 16, 5 ; set MPIDX

    ldsr r0, 20, 5 ; set MPLA
    ldsr r0, 21, 5 ; set MPUA
    ldsr r0, 22, 5 ; set MPAT

    addi 1, r20, r20
    bne _clear_mpu_1
_clear_mpu_exit:
    jmp [lp]
