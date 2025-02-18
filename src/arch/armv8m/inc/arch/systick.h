#ifndef SYSTICK_ARCH_H
#define SYSTICK_ARCH_H

#include <bao.h>
#include <platform.h>
#include <arch/cpu.h>
#include <arch/sysregs.h>

static inline void systick_set_cvr(struct systick* st, uint32_t val)
{
    st->cvr = val & (0xFFFFFFUL);
}

static inline uint32_t systick_get_cvr(struct systick* st)
{
    return st->cvr;
}

static inline void systick_set_rvr(struct systick* st, uint32_t val)
{
    st->rvr = val & (0xFFFFFFUL);
}

static inline uint32_t systick_get_rvr(struct systick* st)
{
    return st->rvr;
}

static inline void systick_set_csr(struct systick* st, uint32_t val)
{
    st->csr = val;
}

static inline uint32_t systick_get_csr(struct systick* st)
{
    return st->csr;
}

#endif /* SYSTICK_ARCH_H */
