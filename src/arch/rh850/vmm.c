/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <vmm.h>

#include <arch/srs.h>

void vmm_arch_init(void)
{
    /* enable virtualization */
    set_hvcfg(1);
}
