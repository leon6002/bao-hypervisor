/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <mem.h>
#include <cpu.h>
#include <vm.h>
#include <arch/sysregs.h>
#include <arch/fences.h>
#include <arch/sau.h>

struct sau_temp {
    unsigned long rbar;
    unsigned long rlar;
} sau_temp[8];

void sau_read_and_save(void);

void sau_read_and_save(void)
{
    for (size_t i = 0; i < 8; i++) {
        sau->rnr = i;
        sau_temp[i].rbar = sau->rbar & SAU_RBAR_BADDR_MSK;

        if (sau_temp[i].rbar & 0x1) {
            sau_temp[i].rlar = sau->rlar | 0x1F;
        } else {
            sau_temp[i].rlar = sau->rlar;
        }
    }
}

static inline size_t sau_num_entries(void)
{
    return (size_t)SAU_TYPE_N_RGN(sau->type);
}

static inline void sau_lock_entry(mpid_t mpid)
{
    bitmap_set(cpu()->vcpu->arch.sau_vm.locked, mpid);
}

static inline bool sau_entry_locked(mpid_t mpid)
{
    return !!bitmap_get(cpu()->vcpu->arch.sau_vm.locked, mpid);
}

static void sau_entry_set(mpid_t mpid, struct mp_region* mpr)
{
    unsigned long lim = mpr->base + mpr->size - 1;

    sau->rnr = mpid;
    ISB();
    sau->rbar = (mpr->base & SAU_RBAR_BADDR_MSK);
    sau->rlar = (lim & SAU_RLAR_LADDR_MSK) | mpr->mem_flags.rlar;
}

static mpid_t sau_entry_allocate(void)
{
    mpid_t reg_num = INVALID_MPID;
    for (mpid_t i = 0; i < (mpid_t)sau_num_entries(); i++) {
        if (bitmap_get(cpu()->vcpu->arch.sau_vm.bitmap, i) == 0) {
            bitmap_set(cpu()->vcpu->arch.sau_vm.bitmap, i);
            reg_num = i;
            break;
        }
    }
    return reg_num;
}

bool sau_add_region(struct mp_region* reg, bool locked)
{
    bool failed = true;

    if (reg->size > 0) {
        mpid_t mpid = sau_entry_allocate();

        if (mpid != INVALID_MPID) {
            failed = false;
            sau_entry_set(mpid, reg);
            if (locked) {
                sau_lock_entry(mpid);
            }
        }
    }

    sau_read_and_save();

    return !failed;
}

bool sau_perms_compatible(mem_flags_t perms1, mem_flags_t perms2)
{
    UNUSED_ARG(perms1);
    UNUSED_ARG(perms2);
    // TODO:ARMV8M - IMPLEMENT on all archs
    // uint8_t perms_mask = SPMPCFG_S_BIT | SPMPCFG_R_BIT | SPMPCFG_W_BIT | SPMPCFG_X_BIT;
    // return (perms1 & perms_mask) == (perms2 & perms_mask);

    return 1;
}

static void sau_entry_get_region(mpid_t mpid, struct mp_region* mpe)
{
    sau->rnr = mpid;
    ISB();

    unsigned long rbar = sau->rbar;
    unsigned long rlar = sau->rlar;

    mpe->mem_flags.rlar = SAU_RLAR_FLAGS(rlar);
    mpe->base = SAU_RBAR_BASE(rbar);
    mpe->size = (SAU_RLAR_LIMIT(rlar) + 1) - mpe->base;
    mpe->as_sec = SEC_UNKNOWN;
}

static mpid_t sau_entry_get_region_id(struct mp_region* mpe)
{
    mpid_t mpid = INVALID_MPID;

    for (mpid_t i = 0; i < (mpid_t)sau_num_entries(); i++) {
        struct mp_region mpe_cmp;
        sau_entry_get_region(i, &mpe_cmp);

        if (mpe_cmp.base == mpe->base && mpe_cmp.size == mpe->size) {
            mpid = i;
            break;
        }
    }

    return mpid;
}

static void sau_entry_clear(mpid_t mpid)
{
    sau->rnr = mpid;
    ISB();
    sau->rlar = 0;
    sau->rbar = 0;
}

static inline void sau_entry_free(mpid_t mpid)
{
    sau_entry_clear(mpid);
    bitmap_clear(cpu()->vcpu->arch.sau_vm.bitmap, mpid);
}

bool sau_remove_region(struct mp_region* reg)
{
    bool failed = true;

    if (reg->size > 0) {
        mpid_t mpid = sau_entry_get_region_id(reg);

        if (mpid != INVALID_MPID) {
            failed = false;
            sau_entry_free(mpid);
        }
    }
    // TODO:ARMV8M - REMOVE
    sau_read_and_save();

    return !failed;
}

bool sau_update_region(struct mp_region* mpr)
{
    bool failed = true;

    for (mpid_t mpid = 0; mpid < (mpid_t)sau_num_entries(); mpid++) {
        if (bitmap_get(cpu()->vcpu->arch.sau_vm.bitmap, mpid) == 0) {
            continue;
        }
        struct mp_region mpe_cmp;
        sau_entry_get_region(mpid, &mpe_cmp);

        if (mpe_cmp.base == mpr->base) {
            sau_entry_set(mpid, mpr);
            failed = false;
            break;
        }
    }
    // TODO:ARMV8M - REMOVE
    sau_read_and_save();

    return !failed;
}

static inline bool sau_entry_valid(mpid_t mpid)
{
    sau->rnr = mpid;
    ISB();
    return !!(sau->rlar & SAU_RLAR_EN);
}

void sau_arch_init(void)
{
    bitmap_clear_consecutive(cpu()->vcpu->arch.sau_vm.bitmap, 0, sau_num_entries());

    for (mpid_t mpid = 0; mpid < (mpid_t)sau_num_entries(); mpid++) {
        if (sau_entry_valid(mpid)) {
            bitmap_set(cpu()->vcpu->arch.sau_vm.bitmap, mpid);
            bitmap_set(cpu()->vcpu->arch.sau_vm.locked, mpid);
        }
    }
}

void sau_arch_enable(void)
{
    sau->ctrl |= SAU_CTRL_ENABLE;
    ISB();

    sau_read_and_save();
}
