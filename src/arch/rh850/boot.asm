
.section ".data", data
.align 2

.public __load_addr
__load_addr:
    .db2 0x10

.public __data_addr
__data_addr:
    .db2 0x10

.section	".text", text
.align	2
__start:
    di ; Disable interrupts
    ; Disable faults ?

    stsr 0, r1, 2 ; get PEID (regID 0, selID 2)

    mov #_hyp_vector_table, r2
    ldsr r2, 2, 1 ; set RBASE (regID 2, selID 1)
    ldsr r2, 3, 1 ; set EBASE (regID 3, selID 1)

    mov #_hyp_interrupt_table, r2
    ldsr r2, 4, 1 ; set INTBP (regID 4, selID 1)

    ; identify master cpu


    ; disable memory protections
    mov r0, r2 ; MPM.MPE disabled
    ldsr r2, 0, 5 ; set MPM

    ; cover all memory with protection check
    ldsr r0, 8, 5 ; Set the address to MCA
    ldsr r0, 9, 5 ; Set the size to MCS

    ; individual protections per cpu TODO should it be otherwise?
    ldsr r1, 12, 5 ; set MCI (one spmid per cpu)

    ; initialize mpu entries for bank 0 TODO use other banks?
    ldsr r0, 17, 5 ; set MPBK
    jarl clear_mpu, lp

    ; CPU physical based address
    mov #__s.bss, r10

    ; CPUx physical based address
    mov 3768, r12 ; CPU_SIZE
    mulh r12, r1
    add r12, r10

    ; clear CPUx struct
    mov r10, r11
    ; r11: start r12: end
    jarl boot_clear, lp

    ; enable faults ?

    ; copy non .text segments to ram

    ; clear .bss
    mov #__s.bss, r11
    mov #__e.bss, r12
    jarl boot_clear, lp

    ; Initialize stack pointer
    mov 3768, r4 ; CPU_STACK_OFF
    add r10, r4 
    mov 4096, r5
    add r5, r4
    mov r4, sp ; set sp
    mov #__start, r5 ; set Text Pointer

    br _init

clear_mpu:
    mov r0, r5
    ; TODO clear both banks

clear_mpu_1:
    addi 1, r5, r5
    ldsr r5, 16, 5 ; set MPIDX

    ldsr r0, 20, 5 ; set MPLA
    ldsr r0, 21, 5 ; set MPUA
    ldsr r0, 22, 5 ; set MPAT

    cmp 32, r5  ; TODO # of mpu entries platform defined
    bne clear_mpu_1
    jmp [lp]

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

