/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <asm_defs.h>
#include <arch/sysregs.h>
#include <arch/interrupts.h>

/*
 * TODO:ARMV8M
 * In Armv8-M the vector table is naturally aligned to a power of two, with an alignment value that
 * is, e.g., 128 bytes for CM33, 256 bytes for CM23. As such, this needs to be cpu-specific.
 *
 * The number of interrupts is also cpu-specific, e.g., 480 for CM33, 240 for CM23.
 */
#define VECTTBL_ALIGN   (0x80)

.macro define_handler name
    .thumb
    .thumb_func
    .global \name
    \name:
.endm

.syntax unified
.text

.macro VM_EXIT
    /* Read CPU pointer (psp) to r0 */
    mrs r1, psp
    ldr r0, [r1, #CPU_VCPU_OFF]

    //TODO:ARMV8M - This should be done only if we are scheduling another VM
    /* Save all special registers */
    add r0, #(VCPU_SPREGS_OFF+VCPU_SPREGS_SIZE)
    stmdb r0!, {r4-r11,r14}

    mrs r1, msp_ns
    mrs r2, psp_ns
    mrs r3, msplim_ns
    mrs r4, psplim_ns
    mrs r5, basepri_ns
    mrs r6, primask_ns
    mrs r7, faultmask_ns
    mrs r8, control_ns

    /* Save all registers */
    stmia   r0!, {r1-r8}
.endm

.macro VM_ENTRY
    /* Read CPU pointer (psp) to r0 */
    mrs r1, psp
    ldr r0, [r1, #CPU_NEXT_VCPU_OFF]
    cmp r0, #0
    beq 11f
    ldr r2, [r1, #CPU_VCPU_OFF]
    cmp r0, r2
    beq 1f
    bl vcpu_context_switch

1:
    ldr r0, [r1, #VCPU_BLOCKED_COUNT]
    cmp r1, #0
    beq 2f
11:
    bl cpu_arch_park
    b .
2:
    ldr r0, [r1, #(VCPU_SPREGS_OFF+VCPU_SPREGS_SIZE)]
    /* Restore sp_regs */
    ldmdb r0!, {r1-r8}

    msr msp_ns,       r1  /* r1 (MSP)         */
    msr psp_ns,       r2  /* r2 (PSP)         */
    msr msplim_ns,    r3  /* r3 (MSPLIM)      */
    msr psplim_ns,    r4  /* r4 (PSPLIM)      */
    msr basepri_ns,   r5  /* r5 (BASEPRI)     */
    msr primask_ns,   r6  /* r6 (PRIMASK)     */
    msr faultmask_ns, r7  /* r7 (FAULTMASK)   */
    msr control_ns,   r8  /* r8 (CONTROL)     */

    /* Restore all registers */
    ldmdb r0!, {r4-r11, lr}                     /* R4-R11, R14 */

    //TODO:ARMV8-M - Still missing some procedures (Security Extensions on Dev Tools)

    /* Jump to VM */
    bx lr
    b   .
.endm


.balign VECTTBL_ALIGN
.global _hyp_vector_table
_hyp_vector_table:
    /* Exceptions */
    /*
     * In Armv8-M, the processor fetches the first two words of the vector table at each power-on/
     * reset sequence. The first is initial value of the MSP (stack pointer) and the second is the
     * reset vector. The reset vector is the address of the reset handler.
     */
    .word 0                     //TODO:ARMV8M should this be initialized with the real MSP value?
    .word _reset_handler
    .rept 13
    .word _exception_handler
    .endr
    .word _systick_handler
    /* Interrupts */
    .rept MAX_INTERRUPTS
    .word _irq_handler
    .endr


/* Handlers */
define_handler _exception_handler
_exception_handler:
    b   fault_exception_handler
    b   .

define_handler _irq_handler
_irq_handler:
    b   .

define_handler _systick_handler
_systick_handler:
    VM_EXIT
    mov r0, #SYSTICK_INT_N
    bl interrupts_handle

.global vcpu_arch_vm_entry
vcpu_arch_entry:
    VM_ENTRY
    cmp r0, #0
    beq first_entry
excep_return:
    bx lr
first_entry:
    ldr r0, [r1, #(VCPU_REGS_PC_OFF)]
    bxns r0

