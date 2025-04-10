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

extern unsigned int boot_ctrl;

#pragma inline_asm snooze
void snooze(void)
{
    SNOOZE
}

/* Perform architecture dependent cpu cores initializations */
void cpu_arch_init(cpuid_t cpuid, paddr_t load_addr)
{
    volatile unsigned int* bootcrl = BOOT_CTRL;

    if (cpuid == CPU_MASTER) {
        for (size_t c = 0; c < platform.cpu_num; c++) {
            if (c == cpuid) {
                continue;
            }

            // TODO: Manual recommends 100us wait time between each core
            /* We don't have MPU setup yet so it's safe to use direct pointers */
            // volatile unsigned int* bootcrl = BOOT_CTRL;
            (*bootcrl) |= (1 << c);
        }
    }

    (*bootcrl) = 0x3;
    boot_ctrl = (*bootcrl);

    /* clear exception registers */
    set_eipc(0x0);
    set_fepc(0x0);
    set_mea(0x0);
    set_mei(0x0);
    set_eiic(0x0);
    set_feic(0x0);
};

inline struct cpu* cpu(void)
{
    return (struct cpu*)get_fewr();
}

#pragma inline_asm reset_stack_and_jump
static void reset_stack_and_jump(void* stack_base, void* jmp_target)
{
    mov r6, sp
    pushsp lp, lp
    jarl [r7], lp
    popsp lp, lp
    jmp [lp]
}


void cpu_arch_standby()
{
    snooze();
    reset_stack_and_jump(&cpu()->stack[STACK_SIZE], cpu_standby_wakeup);
    ERROR("returned from standby wake up");
}

void cpu_arch_powerdown()
{
    snooze();
    reset_stack_and_jump(&cpu()->stack[STACK_SIZE], cpu_powerdown_wakeup);
    ERROR("returned from powerdown wake up");
}
