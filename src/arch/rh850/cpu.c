/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <bao.h>
#include <cpu.h>
#include <platform.h>
#include <arch/sysregs.h>

cpuid_t CPU_MASTER;

/* Perform architecture dependent cpu cores initializations */
void cpu_arch_init(cpuid_t cpuid, paddr_t load_addr) { }

unsigned long cpu_id_to_mpidr(cpuid_t id)
{
    return 0;
}

void cpu_arch_standby()
{
    ERROR("returned from standby wake up");
}

void cpu_arch_powerdown()
{
    ERROR("returned from powerdown wake up");
}
