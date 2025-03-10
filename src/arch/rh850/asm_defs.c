/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <bao.h>
#include <cpu.h>
#include <vm.h>
#include <platform.h>
#include <arch/sysregs.h>

static void sysregs_defines(void) { }

static void cpu_defines(void)
{
    DEFINE_SIZE(CPU_SIZE, struct cpu);

    DEFINE_OFFSET(CPU_STACK_OFF, struct cpu, stack);
    DEFINE_SIZE(CPU_STACK_SIZE, ((struct cpu*)NULL)->stack);
    DEFINE_OFFSET(CPU_VCPU_OFF, struct cpu, vcpu);
    DEFINE_OFFSET(CPU_NEXT_VCPU_OFF, struct cpu, next_vcpu);
}

static void vcpu_defines(void) { }

static void platform_defines(void) { }
