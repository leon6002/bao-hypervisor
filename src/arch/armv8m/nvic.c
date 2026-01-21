/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <bao.h>
#include <interrupts.h>

#include <cpu.h>
#include <mem.h>
#include <vm.h>
#include <arch/nvic.h>

extern irq_handler_t interrupt_handlers[MAX_INTERRUPT_HANDLERS];

void nvic_init(void) { }

void nvic_int_handle(void) { }
