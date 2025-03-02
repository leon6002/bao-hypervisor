/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <list.h>
#include <mem.h>
#include <cpu.h>
#include <vm.h>
#include <arch/mpu.h>

void as_arch_init(struct addr_space* as)
{
    UNUSED_ARG(as);
}

bool mpu_map(struct addr_space* as, struct mp_region* mpr, bool locked)
{
    bool failed = true;

    return !failed;
}

bool mpu_unmap(struct addr_space* as, struct mp_region* mpr)
{
    bool failed = true;

    return !failed;
}

bool mpu_update(struct addr_space* as, struct mp_region* mpr)
{
    bool failed = true;

    return !failed;
}

bool mpu_perms_compatible(struct addr_space* as, mem_flags_t perms1, mem_flags_t perms2)
{
    bool failed = true;
    return !failed;
}

void mpu_init(void)
{
    mpu_arch_init();
}

void mpu_enable()
{
    mpu_arch_enable();
}
