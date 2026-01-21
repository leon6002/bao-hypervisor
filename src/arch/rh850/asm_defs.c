/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <bao.h>
#include <cpu.h>
#include <stddef.h>
#include <vm.h>
#include <platform.h>

static void sysregs_defines(void) { }

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#undef DEFINE_OFFSET
#define DEFINE_OFFSET(SYMBOL, STRUCT, FIELD) \
    .equ TOSTRING(SYMBOL), TOSTRING(offsetof(STRUCT, FIELD))

#undef DEFINE_SIZE
#define DEFINE_SIZE(SYMBOL, TYPE) \
    .equ SYMBOL, sizeof(TYPE))

const size_t cpu_size = sizeof(struct cpu);
#pragma inline_asm asm_cpu_size
void asm_cpu_size()
{
    .equ CPU_SIZE, cpu_size
}

const size_t cpu_stack_off = offsetof(struct cpu, stack);
#pragma inline_asm asm_cpu_stack_off
void asm_cpu_stack_off()
{
    .equ CPU_STACK_OFF, cpu_stack_off
}

const size_t cpu_stack_size = sizeof(((struct cpu*)NULL)->stack);
#pragma inline_asm asm_cpu_stack_size
void asm_cpu_stack_size()
{
    .equ CPU_STACK_SIZE, cpu_stack_size
}

const size_t cpu_vcpu_off = offsetof(struct cpu, vcpu);
#pragma inline_asm asm_cpu_vcpu_off
void asm_cpu_vcpu_off()
{
    .equ CPU_VCPU_OFF, cpu_stack_off
}

const size_t vcpu_regs_size = sizeof(struct arch_regs);
#pragma inline_asm asm_vcpu_regs_size
void asm_vcpu_regs_size()
{
    .equ VCPU_REGS_SIZE, vcpu_regs_size
}

const size_t vcpu_regs_off = offsetof(struct vcpu, regs);
#pragma inline_asm asm_vcpu_regs_off
void asm_vcpu_regs_off()
{
    .equ VCPU_REGS_OFF, vcpu_regs_off
}

const size_t vcpu_arch_size = sizeof(struct vcpu_arch);
#pragma inline_asm asm_vcpu_arch_size
void asm_vcpu_arch_size()
{
    .equ VCPU_ARCH_SIZE, vcpu_arch_size
}

static void cpu_defines(void)
{
    // DEFINE_SIZE(CPU_SIZE, struct cpu);

    // DEFINE_OFFSET(CPU_STACK_OFF, struct cpu, stack);
    // DEFINE_SIZE(CPU_STACK_SIZE, ((struct cpu*)NULL)->stack);
    // DEFINE_OFFSET(CPU_VCPU_OFF, struct cpu, vcpu);
}

static void vcpu_defines(void)
{
    // DEFINE_SIZE(VCPU_ARCH_SIZE, struct vcpu_arch);
    // DEFINE_OFFSET(VCPU_REGS_OFF, struct vcpu, regs);
    // DEFINE_SIZE(VCPU_REGS_SIZE, struct arch_regs);
}

static void platform_defines(void) { }
