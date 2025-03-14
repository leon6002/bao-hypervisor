/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include "inc/drivers/dummy_uart.h"
#include <drivers/dummy_uart.h>

void uart_init(volatile struct dummy_hw* uart)
{
    uart->count = 0;
}

void uart_enable(volatile struct dummy_hw* uart)
{
    UNUSED_ARG(uart);
}

void uart_putc(volatile struct dummy_hw* uart, int8_t c)
{
    uart->buf[uart->count % DUMMY_BUF_SIZE] = c;
    uart->count++;
}
