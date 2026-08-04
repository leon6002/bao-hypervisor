/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <bao.h>
#include <cpu.h>
#include <vm.h>
#include <platform.h>

/*
 * On CC-RH the DEFINE_* macros expand to file-scope constants that the build reads back out of
 * the generated assembly, so they cannot sit inside a function. Everywhere else they expand to
 * asm comments and need one.
 */
#ifdef CC_IS_RHCC
#define ASM_DEFS_BEGIN(name)
#define ASM_DEFS_END
#else
#define ASM_DEFS_BEGIN(name) __attribute__((used)) static void name(void) {
#define ASM_DEFS_END }
#endif

ASM_DEFS_BEGIN(cpu_defines)
    DEFINE_SIZE(CPU_SIZE, struct cpu);
    DEFINE_OFFSET(CPU_STACK_OFF, struct cpu, stack);
    DEFINE_SIZE(CPU_STACK_SIZE, ((struct cpu*)NULL)->stack);
    DEFINE_OFFSET(CPU_VCPU_OFF, struct cpu, vcpu);
ASM_DEFS_END

ASM_DEFS_BEGIN(vcpu_defines)
    DEFINE_SIZE(VCPU_ARCH_SIZE, struct vcpu_arch);
    DEFINE_OFFSET(VCPU_REGS_OFF, struct vcpu, regs);
    DEFINE_SIZE(VCPU_REGS_SIZE, struct arch_regs);
ASM_DEFS_END

ASM_DEFS_BEGIN(platform_defines)
    DEFINE_OFFSET(PLAT_CPUNUM_OFF, struct platform, cpu_num);
    DEFINE_OFFSET(PLAT_ARCH_OFF, struct platform, arch);

    DEFINE_OFFSET(PLATFORM_REGION_NUM_OFF, struct platform, region_num);
    DEFINE_OFFSET(PLATFORM_REGIONS_PTR_OFF, struct platform, regions);

    DEFINE_OFFSET(MEM_REGION_BASE_OFF, struct mem_region, base);
    DEFINE_OFFSET(MEM_REGION_SIZE_OFF, struct mem_region, size);
    DEFINE_OFFSET(MEM_REGION_PERMS_OFF, struct mem_region, perms);
    DEFINE_SIZE(MEM_REGION_SIZEOF, struct mem_region);
ASM_DEFS_END
