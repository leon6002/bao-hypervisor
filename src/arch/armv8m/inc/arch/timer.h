#ifndef TIMER_ARCH_H
#define TIMER_ARCH_H

#include <bao.h>
#include <platform.h>
#include <arch/cpu.h>
#include <arch/sysregs.h>
#include <arch/systick.h>

typedef uint32_t timer_value_t;

#define TIMER_ARCH_FREQ() (PLAT_TIMER_FREQ)

irqid_t timer_arch_irq_id(void);

static inline void timer_arch_set(timer_value_t value)
{
    systick_set_cvr(systick_s, value);
    systick_set_rvr(systick_s, value);
}

static inline timer_value_t timer_arch_get_count(void)
{
    return (timer_value_t)systick_get_cvr(systick_s);
}

static inline void timer_arch_disable(void)
{
    systick_set_csr(systick_s, systick_get_cvr(systick_s) & ~SYSTICK_CSR_ENABLE);
}

static inline void timer_arch_enable(void)
{
    systick_set_csr(systick_s,
        systick_get_cvr(systick_s) |
            (SYSTICK_CSR_ENABLE | SYSTICK_CSR_TICKINT | SYSTICK_CSR_CLKSOURCE));
}

static inline void timer_arch_init(void)
{
    timer_arch_disable();
}

#endif /* TIMER_ARCH_H */
