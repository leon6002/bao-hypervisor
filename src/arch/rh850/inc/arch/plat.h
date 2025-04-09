/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __ARCH_PLAT_H__
#define __ARCH_PLAT_H__

#include <bao.h>
#include <cpu.h>
#include <platform.h>

// Clock controller
#define MCU_CKSC_DISABLE_REG_PROTECT_VALUE (0xA5A5A501UL)
#define MCU_CKSC_ENABLE_REG_PROTECT_VALUE (0xA5A5A500UL)
#define MCU_CLKKCPROT1 (0xFF980700UL)

#define MCU_MOSCE (0xFF988000UL)
#define MCU_MOSCE_ENABLE_TRIGGER  (0x00000001UL)
#define MCU_MOSCE_DISABLE_TRIGGER (0x00000002UL)
#define MCU_MOSCSTPM (0xFF98800CUL)

#define MCU_PLLE  (0xFF980000UL)
#define MCU_PLLE_ENABLE_TRIGGER  (0x00000001UL)
#define MCU_PLLE_DISABLE_TRIGGER (0x00000002UL)
#define MCU_PLLSTPM  (0xFF98000CUL)

#define MCU_CKSC_CPUC  (0xFF980100UL)

// Ports
#define MCU_PKCPROT (0xFFD92F40UL)
#define MCU_PWE (0xFFD92F44UL)

#define PORT_PWE_ALL_MASK      (0x0BF724F8UL)
#define PORT_PWE_ENABLE_WRITE  (0xA5A5A501UL)
#define PORT_PWE_DISABLE_WRITE (0xA5A5A500UL)


void plat_init(void);

#endif  /* __ARCH_PLAT_H__ */

