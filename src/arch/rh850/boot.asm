;
; SPDX-License-Identifier: Apache-2.0
; Copyright (c) Bao Project and Contributors. All rights reserved.
;
; asrh version of boot.S. Kept as a separate file rather than #ifdef'd into the GNU one because
; the two assemblers disagree about almost every line: comment character, section syntax,
; symbol export, and how a symbol address is materialised.
;
; asrh does not run the preprocessor, so the build expands this with the host cpp first, which
; is what makes the #includes below work.

#include <bao.h>
#include <asm_defs.h>
#include <platform_defs.h>
#include <plat/platform.h>

; asrh has no .macro, so LOAD_ADDR is a preprocessor macro. CC-RH materialises a symbol
; address with a single mov rather than the movhi/movea pair GNU as needs.
; asrh accepts a bare symbol as an address operand, which also keeps the C preprocessor from
; treating a leading # as its stringify operator.
#define LOAD_ADDR(sym, reg) mov sym, reg

    .extern _platform

; This code MUST be at the base of the image: it is bao's entry point, so .boot has to be the
; first section placed. Do not put anything ahead of _reset_handler here.
    .section ".boot", text

    .public __reset_handler
__reset_handler:

    ; No ABI is followed in this boot code. Reserved throughout:
    ;   r10 CPU ID, r11 master CPU ID, r12 CPU struct size, r13 CPU struct pointer
    ;   r6  CPU ID, passed on to init
    ; r20-r29 are scratch in the main flow, r15-r19 carry arguments and results.

    di                              ; disable interrupts

    stsr 0, r10, 2                  ; current CPU ID, from PEID

    mov CPU_MASTER_FIXED, r11       ; identify master cpu

    mov 0x8020, r20                 ; PSW.EBV and PSW.ID
    ldsr r20, 5, 0

    LOAD_ADDR(_hyp_vector_table, r20)
    ori 0x2, r20, r20               ; direct vector method
    ldsr r20, 3, 1                  ; EBASE

    mov r0, r20                     ; clear MPM, disabling memory protection
    ldsr r20, 0, 5

    ldsr r0, 8, 5                   ; MCA = MCS = 0 covers all memory
    ldsr r0, 9, 5
    ldsr r0, 12, 5                  ; MCI = 0
    ldsr r10, 0, 1                  ; SPID = CPU ID

    ldsr r0, 17, 5                  ; MPBK.BK = 0
    jarl __clear_mpu, lp

    ; Interrupt virtualisation support, recommended right after reset.
    mov PLAT_INTC1_ADDR, r20
    mov 0x1, r21
    st.w r21, PLAT_INTC1_IHVCFG_OFFSET[r20]

    cmp r11, r10                    ; only the master initialises memory
    bne __clear_cpu

    LOAD_ADDR(_platform, r15)
    LOAD_ADDR(__data_vma_start, r16)
    LOAD_ADDR(__data_lma_start, r17)
    jarl _clear_rwx_regions, lp

    LOAD_ADDR(__data_lma_start, r15)    ; copy non-text sections from flash to ram
    LOAD_ADDR(__image_load_end, r16)
    mov PLAT_DATA_ADDR, r17
    jarl __copy_data, lp

    LOAD_ADDR(__image_noload_start, r15) ; clear .bss in ram
    LOAD_ADDR(__image_end, r16)
    jarl __boot_clear, lp

    LOAD_ADDR(__image_start, r20)       ; publish the image load address
    LOAD_ADDR(_img_addr, r21)
    st.w r20, 0[r21]

    mov PLAT_DATA_ADDR, r20             ; publish the data section address
    LOAD_ADDR(_data_addr, r21)
    st.w r20, 0[r21]

__clear_cpu:
    LOAD_ADDR(__image_end, r20)         ; CPUn struct sits past the end of .bss
    mov CPU_SIZE, r12
    mov r10, r21
    mulh r12, r21
    add r21, r20
    mov r20, r13                        ; r13 holds the CPU pointer
    mov r13, r15
    mov r13, r16
    add r12, r16
    jarl __boot_clear, lp

    ldsr r13, 29, 0                     ; FEWR holds the CPU* for cpu()

    mov CPU_STACK_OFF, r20              ; stack pointer
    add r13, r20
    mov CPU_STACK_SIZE, r21
    add r21, r20
    mov r20, sp

    mov r10, r6                         ; init(cpu_id)
    jr _init

; Clear the RWX memory regions described by the platform.
;   r15 = platform pointer as linked for VMA
;   r16 = start of .data at VMA
;   r17 = start of .data at LMA
    .public _clear_rwx_regions
_clear_rwx_regions:
    mov     r17, r20                    ; delta = data_lma - data_vma
    sub     r16, r20

    mov     r15, r21                    ; platform pointer, converted to LMA
    add     r20, r21

    ld.w    PLATFORM_REGION_NUM_OFF[r21], r24
    cmp     r0, r24
    be      __done                      ; nothing to do

    ld.w    PLATFORM_REGIONS_PTR_OFF[r21], r18
    add     r20, r18                    ; regions pointer, converted to LMA

    mov     MEM_REGION_SIZEOF, r25

__loop_region:
    ld.w    MEM_REGION_PERMS_OFF[r18], r26
    cmp     r0, r26                     ; assumes MEM_RWX is 0
    bne     __skip_clear

    ld.w    MEM_REGION_BASE_OFF[r18], r22
    ld.w    MEM_REGION_SIZE_OFF[r18], r23
    cmp     r0, r23
    be      __skip_clear

    add     r22, r23                    ; end = base + size
    br      __clear_rwx_range

__skip_clear:
    add     r25, r18                    ; next region
    addi    -1, r24, r24
    cmp     r0, r24
    bne     __loop_region

__done:
    jmp     [lp]

; Clear words in [r22, r23).
__clear_rwx_range:
    cmp     r22, r23
    be      __cr_done
    st.w    r0, 0[r22]
    addi    4, r22, r22
    br      __clear_rwx_range

__cr_done:
    br      __skip_clear

; Clear [r15, r16).
__boot_clear:
    cmp r16, r15
    bge __boot_clear_exit
    st.w r0, 0[r15]
    addi 4, r15, r15
    br __boot_clear
__boot_clear_exit:
    jmp [lp]

; Copy [r15, r16) to [r17).
__copy_data:
    ld.w 0[r15], r18
    st.w r18, 0[r17]
    addi 4, r15, r15
    addi 4, r17, r17
    cmp r15, r16
    bne __copy_data
    jmp [lp]

; Clear the MPU system registers. Uses r15 and r16.
__clear_mpu:
    mov r0, r15
    stsr 2, r16, 5                  ; MPCFG
    andi 0x1F, r16, r16             ; NMPUE
    addi 1, r16, r16                ; entry count
__clear_mpu_1:
    cmp r15, r16
    be __clear_mpu_exit

    ldsr r15, 16, 5                 ; MPIX
    ldsr r0, 20, 5                  ; MPLA
    ldsr r0, 21, 5                  ; MPUA
    ldsr r0, 22, 5                  ; MPAT

    addi 1, r15, r15
    br __clear_mpu_1
__clear_mpu_exit:
    jmp [lp]
