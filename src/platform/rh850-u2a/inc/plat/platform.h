/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __PLAT_PLATFORM_H__
#define __PLAT_PLATFORM_H__

#include <drivers/renesas_rlin3.h>

#ifndef __ASSEMBLER__
#endif

#define PLAT_MAX_INTERRUPTS     768

#define PLAT_MAX_MPU_REGIONS    32

#define PLAT_NUM_PORT_REGS      21
#define PLAT_NUM_PORT_GROUPS    11
#define PLAT_PORT_BASE          0xFFD90000UL

#endif /* __PLAT_PLATFORM_H__ */
