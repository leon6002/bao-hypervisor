/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <mem.h>
#include <cpu.h>
#include <arch/sysregs.h>
#include <arch/fences.h>
#include <arch/mpu.h>

struct mpu_temp {
    unsigned long rbar;
    unsigned long rlar;
} mpu_temp[8];

bool mpu_add_region(struct mp_region* reg, bool locked)
{
    bool failed = true;

    return !failed;
}

bool mpu_arch_perms_compatible(mem_flags_t perms1, mem_flags_t perms2)
{
    UNUSED_ARG(perms1);
    UNUSED_ARG(perms2);

    return 1;
}

bool mpu_remove_region(struct mp_region* reg)
{
    bool failed = true;

    return !failed;
}

bool mpu_update_region(struct mp_region* mpr)
{
    bool failed = true;

    return !failed;
}

void mpu_arch_init(void) { }

void mpu_arch_enable(void) { }
