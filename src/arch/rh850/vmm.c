/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include "inc/arch/srs.h"
#include <vmm.h>

#include <arch/srs.h>

void vmm_arch_init(void)
{
    /* HVCFG.HVE is set after reset */

    /* set GMCFG.GMP, GMCFG.HMP and GMCFG.GSYSE */
    set_gmcfg(0x13);
    if (get_gmcfg() != 0x13){
        ERROR("GMCFG is not being written");
    }

    /* set EIPSWH.GM */
    set_eipsw(0x80000000);
    if (get_eipswh() != 0x80000000){
        ERROR("EIPSWH is not being written");
    }

    // TODO: set FEPSWH ?

    /* set GMMPM.GMPE */
    set_gmmpm(0x4);
    if (get_gmmpm() != 0x4){
        ERROR("GMMPM is not being written");
    }
}
