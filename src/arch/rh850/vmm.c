/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include "inc/arch/srs.h"
#include <vmm.h>

#include <arch/srs.h>

void vmm_arch_init(void)
{
    /* enable virtualization */
    set_hvcfg(1);

    /* set syserr, MPU, and GMP to trap to host */
    set_gmcfg(0x13);
}
