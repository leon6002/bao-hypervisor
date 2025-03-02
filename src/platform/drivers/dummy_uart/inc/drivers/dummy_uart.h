/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef UARTDUMMY_H
#define UARTDUMMY_H

#include <bao.h>
#include <plat/platform.h>

struct dummy_hw {
    uint32_t c;
};

typedef struct dummy_hw bao_uart_t;

void uart_enable(volatile struct dummy_hw* uart);
void uart_init(volatile struct dummy_hw* uart);
void uart_putc(volatile struct dummy_hw* uart, int8_t c);

#endif /* UARTDUMMY_H */
