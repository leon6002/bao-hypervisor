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

.public _hyp_interrupt_table
.align	512
_hyp_interrupt_table:
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

	.section "EINTTBL", const ; External interrupt table
	.align	512
	.dw	#_Interrupt_EI ; INT0
	.dw	#_Interrupt_EI ; INT1
	.dw	#_Interrupt_EI ; INT2
	.rept	512 - 3
	.dw	#_Interrupt_EI ; INTn
	.endm

	.section ".text", text
	.align	2

VM_EXIT .macro
    ldsr r31, 28, 0 ; use EIWR as scratchpad
    stsr 29, r31, 0 ; get cpu* from EBASE
    add 8, r31 ; CPU_VCPU_OFF
    ld.w [r31], r31 ; cpu.VCPU
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
    ; TODO PC etc ?

    mov r31, r30
    stsr 28, r31, 0
    st.w r31, 124[r30]
.endm

VM_ENTRY .macro
    stsr 29, r31, 0
    add 8, r31 ; CPU_VCPU_OFF
    ld.w [r31], r31 ; cpu.VCPU
    add 4, r31 ; VCPU_REGS_OFF
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
    ld.w 128[r31], r30
    ldsr r30, 0, 0 ; set EIPC

    ld.w 120[r31], r30
    ld.w 124[r31], r31
    eiret
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
    br _Interrupt_EI   
    ;VM_EXIT
    ; br	_interrupts_handle, lp
    ;VM_ENTRY


    .public _vcpu_arch_entry
_vcpu_arch_entry:
    VM_ENTRY
vcpu_arch_entry_1:
    br	vcpu_arch_entry_1
