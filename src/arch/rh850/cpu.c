/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <bao.h>
#include <cpu.h>
#include <platform.h>
#include <arch/srs.h>

cpuid_t CPU_MASTER;

#define BOOT_CTRL ((unsigned int*)0xFFFB2000)

#pragma inline_asm snooze
void snooze(void)
{
    SNOOZE
}

/* Perform architecture dependent cpu cores initializations */
void cpu_arch_init(cpuid_t cpuid, paddr_t load_addr)
{
    if (cpuid == CPU_MASTER) {
        for (size_t c = 0; c < platform.cpu_num; c++) {
            if (c == cpuid) {
                continue;
            }
            /* We don't have MPU setup yet so it's safe to use direct pointers */
            volatile unsigned int* bootcrl = BOOT_CTRL;
            (*bootcrl) |= (1 << cpuid);
        }
    }
}

inline struct cpu* cpu(void)
{
    return (struct cpu*)get_ebase();
}

void cpu_arch_standby()
{
    snooze();
}

void cpu_arch_powerdown()
{
    snooze();
    ERROR("returned from powerdown wake up");
}
