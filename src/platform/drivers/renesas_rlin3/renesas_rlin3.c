/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <drivers/renesas_rlin3.h>

void uart_init(volatile struct renesas_rlin3* uart)
{
    // enable temporary access to MSRKCPROT and MSR_RLIN3 registers 
    vaddr_t msrkcprot_ptr = mem_alloc_map_dev(&cpu()->as, SEC_HYP_PRIVATE, INVALID_VA,
                            (paddr_t)(MSRKCPROT), 1);
    if (msrkcprot_ptr == INVALID_VA) {
        ERROR("maping MSRKCPROT register failed");
    }
    vaddr_t msr_rlin3_ptr = mem_alloc_map_dev(&cpu()->as, SEC_HYP_PRIVATE, INVALID_VA,
                            (paddr_t)(MSR_RLIN3), 1);
    if (msr_rlin3_ptr == INVALID_VA) {
        ERROR("maping MSR_RLIN3 register failed");
    }

    *((volatile uint32_t*) MSRKCPROT) = KCPROT_ENABLE;
    *((volatile uint32_t*) MSR_RLIN3) = 0;
    *((volatile uint32_t*) MSRKCPROT) = KCPROT_DISABLE;

    mem_unmap(&cpu()->as, (vaddr_t)(MSRKCPROT), 1, true);
    mem_unmap(&cpu()->as, (vaddr_t)(MSR_RLIN3), 1, true);

    // Set reset mode
    uart->RLN3nLCUC = 0;

    while ((uart->RLN3nLMST & RLIN3_LMST_OMM0_MSK) != 0x0)
        ;

    // Set baud rate to 500000 assuming CLK_RLIN = 80 MHz
    uart->RLN3nLWBR = 0; // prescaler reset;
    uart->RLN3nLWBR = RLIN3_LWBR_LPRS_16 | RLIN3_LWBR_NSPB_10;
    // uart->RLN3nLWBR = 0x76;
    uart->RLN3nLBRP01 = 0;
    // uart->RLN3nLBRP01 = 0x81;

    // Set data format
    uart->RLN3nLBFC = 
        RLN3_LBFC_UBLS_8B |
        RLN3_LBFC_UBOS_LSB |
        RLN3_LBFC_USBLS_1B |
        RLN3_LBFC_UPS_DIS;
 
    // Set uart mode
    uart->RLN3nLMD = RLN3_LMD_UART_MODE; 
    uart->RLN3nLCUC = RLN3_LCUC_LIN_CANC;

    while ((uart->RLN3nLMST & RLIN3_LMST_OMM0_MSK) != 0x1)
        ;
}

void uart_enable(volatile struct renesas_rlin3* uart)
{
    uart->RLN3nLUOER = RLN3_LUOER_UROE | RLN3_LUOER_UTOE;
}

void uart_putc(volatile struct renesas_rlin3* uart, int8_t c)
{
    while (uart->RLN3nLST & RLN3_LST_UTS_MSK);
    uart->RLN3nLUTDR = c;
}
 