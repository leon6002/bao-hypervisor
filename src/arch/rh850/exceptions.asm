.section "VECTAB", text
.public _hyp_vector_table
.extern __start
_hyp_vector_table:
	.align	512
	jr32	__start ; RESET

	.align	16
	syncp
	jr32	_guest_exception ; SYSERR ; Guest

	.align	16
	jr32	_host_exception ; (R.F.U)

	.align	16
	jr32	_guest_exception ; FETRAP ; Guest?

	.align	16
	jr32	_guest_exception ; TRAP0 ; Guest?

	.align	16
	jr32	_guest_exception ; TRAP1 ; Guest?

	.align	16
	jr32	_host_exception ; RIE

	.align	16
	syncp
	jr32	_host_exception ; FPP/FPI

	.align	16
	jr32	_host_exception ; UCPOP

	.align	16
	jr32	_guest_exception ; MIP/MDP ; Guest

	.align	16
	jr32	_host_exception ; RIE

	.align	16
	jr32	_host_exception ; PIE

	.align	16
	jr32	_host_exception ; MAE

	.align	16
	jr32	_host_exception ; UCPOP

	.align	16
	syncp
	jr32	_host_exception ; FENMI

	.align	16
	syncp
	jr32	_host_exception ; FEINT

	; only used with direct vector method
	.align	16
	syncp
	jr32	_Interrupt_EI ; INTn(priority0)

	.align	16
	syncp
	jr32	_Interrupt_EI ; INTn(priority1)

	.align	16
	syncp
	jr32	_Interrupt_EI ; INTn(priority2)

	.align	16
	syncp
	jr32	_Interrupt_EI ; INTn(priority3)

	.align	16
	syncp
	jr32	_Interrupt_EI ; INTn(priority4)

	.align	16
	syncp
	jr32	_Interrupt_EI ; INTn(priority5)

	.align	16
	syncp
	jr32	_Interrupt_EI ; INTn(priority6)

	.align	16
	syncp
	jr32	_Interrupt_EI ; INTn(priority7)

	.align	16
	syncp
	jr32	_Interrupt_EI ; INTn(priority8)

	.align	16
	syncp
	jr32	_Interrupt_EI ; INTn(priority9)

	.align	16
	syncp
	jr32	_Interrupt_EI ; INTn(priority10)

	.align	16
	syncp
	jr32	_Interrupt_EI ; INTn(priority11)

	.align	16
	syncp
	jr32	_Interrupt_EI ; INTn(priority12)

	.align	16
	syncp
	jr32	_Interrupt_EI ; INTn(priority13)

	.align	16
	syncp
	jr32	_Interrupt_EI ; INTn(priority14)

	.align	16
	syncp
	jr32	_Interrupt_EI ; INTn(priority15)

; only used with table reference method
.section "EINTTBL", const ; External interrupt table
.public _hyp_interrupt_table
_hyp_interrupt_table:
	.align	512
	.dw	#_Interrupt_EI ; INT0
	.dw	#_Interrupt_EI ; INT1
	.dw	#_Interrupt_EI ; INT2
	.rept	2048 - 3
	.dw	#_Interrupt_EI ; INTn
	.endm

.section ".text", text
	.align	2

; r30: vcpu register struct base address
; uses r20
save_srs:
	stsr 5, r20, 0
	andi 0x80, r20, r20 ; PSW.NP
	cmp r0, r20
	be save_ei
	stsr 2, r20, 0 ; save FEPC
	br save_srs_exit
save_ei:
	stsr 0, r20, 0 ; save EIPC
save_srs_exit:
	st.w r20, 128[r30]
	jmp [lp]

; r30: vcpu register struct base address
; uses r20 and r21
restore_srs:
	ld.w 128[r30], r20 ; PC value
	stsr 5, r21, 0
	andi 0x80, r21, r21 ; PSW.NP (we assume it did not change)
	cmp r0, r21
	be restore_ei
	ldsr r20, 2, 0 ; set FEPC
	br restore_srs_exit
restore_ei:
	ldsr r20, 2, 0 ; set EIPC
restore_srs_exit:
	jmp [lp]

select_xxret:
	stsr 5, r31, 0
	andi 0x80, r31, r31 ; PSW.NP
	cmp r0, r31
	be ei_ret
	stsr 28, r31, 0
    feret
ei_ret:
	stsr 28, r31, 0
	eiret

VM_EXIT .macro

	; set SPID to HYP_SPID
	ldsr r31, 28, 0 ; use EIWR as scratchpad
	mov 0x0, r31 ; HYP_SPID
	ldsr r31, 0, 1

	; save program registers in vcpu struct
    stsr 29, r31, 0 ; get cpu* from FEWR
    addi 0x828, r31, r31 ; CPU_VCPU_OFF
    ld.w [r31], r31 ; get cpu.VCPU*
    add 4, r31 ; VCPU_REGS_OFF
    st.dw r0,  0[r31]
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
    st.w r30, 120[r31]

    mov r31, r30
    stsr 28, r31, 0
    st.w r31, 124[r30]

	; save PC
	jarl save_srs, lp

	; restore stack pointer
	stsr 29, r20, 0 ; get cpu* from FEWR
    mov 0x838, r21 ; CPU_STACK_OFF
    add r21, r20 ; add stack offset to CPU pointer
    mov 0x1000, r21 ; CPU STACK SIZE
    add r21, r20 ; stack base + stack size
    mov r20, sp ; set sp
.endm

VM_ENTRY .macro
	; load vcpu registers from memory
    stsr 29, r30, 0
    addi 0x828, r30, r30 ; CPU_VCPU_OFF
    ld.w [r30], r30 ; cpu.VCPU
    add 4, r30 ; VCPU_REGS_OFF

	; restore PC
	jarl restore_srs, lp
	mov r30, r31

    ld.dw 0[r31], r0
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
    ld.dw 120[r31], r30

	; set SPID to VM_SPID
	ldsr r31, 28, 0
	mov 0x1, r31 ; VM_SPID
	ldsr r31, 0, 1

	;; the instructions below should not trigger any MIP exception, 
	;; as all the hypervisor code belongs to an MPU entry with RG = 1.

	; select ret instruction
	br select_xxret
.endm


.extern _abort
.extern _interrupts_handle

_guest_exception: 

    VM_EXIT
    jarl _abort, lp
    VM_ENTRY


_host_exception:
	br	_host_exception

_Interrupt_EI:

    VM_EXIT
	
	; copy int_id to r6
	stsr 13, r6, 0 ; get int cause from EEIC
	mov 0x7FF, r20
	and r20, r6 ; mask EIINT source

    jarl _interrupts_handle, lp

    VM_ENTRY


.public _vcpu_arch_entry
_vcpu_arch_entry:
    VM_ENTRY
vcpu_arch_entry_1:
    br	vcpu_arch_entry_1
