/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <bao.h>
#include <cpu.h>
#include <platform.h>
#include <arch/srs.h>

cpuid_t CPU_MASTER;

/* Perform architecture dependent cpu cores initializations */
void cpu_arch_init(cpuid_t cpuid, paddr_t load_addr)
{
    if (cpuid == CPU_MASTER) {
        for (size_t c = 0; c < platform.cpu_num; c++) {
            if (c == cpuid) {
                continue;
            }
            WARNING("waking CPU %d not implemented", c);
        }
    }
}

unsigned long cpu_id_to_mpidr(cpuid_t id)
{
    return 0;
}

inline struct cpu* cpu(void)
{
    return (struct cpu*)get_ebase();
}

void cpu_arch_standby()
{
    ERROR("returned from standby wake up");
}

void cpu_arch_powerdown()
{
    ERROR("returned from powerdown wake up");
}
