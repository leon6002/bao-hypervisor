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
    set_gmcfg(0x12);
    if (get_gmcfg() != 0x12){
        ERROR("GMCFG is not being written");
    }

    /* set EIPSWH.GM */
    set_eipswh(0x80000000);
    if (get_eipswh() != 0x80000000){
        ERROR("EIPSWH is not being written");
    }

    /* set FEPSWH.GM */
    set_fepswh(0x80000000);
    if (get_fepswh() != 0x80000000){
        ERROR("FEPSWH is not being written");
    }

    /* set GMMPM.GMPE */
    set_gmmpm(0x4);
    if (get_gmmpm() != 0x4){
        ERROR("GMMPM is not being written");
    }

    /* set GMSPIDLIST with available SPIDs not used by the hyp */
    set_gmspidlist(0x0);

    /* set GMSPID */
    set_gmspid(VM_SPID);

    /* clear guest-context exception registers */
    set_gmeipc(0x0);
    set_gmfepc(0x0);
    set_gmmea(0x0);
    set_gmmei(0x0);
    set_gmeiic(0x0);
    set_gmfeic(0x0);
}
