#ifndef TIMER_ARCH_H
#define TIMER_ARCH_H

#include <bao.h>
#include <platform.h>
#include <arch/cpu.h>

typedef uint32_t timer_value_t;

#define TIMER_ARCH_FREQ() (0x0)

irqid_t timer_arch_irq_id(void);

static inline void timer_arch_enable(void) { }

static inline void timer_arch_disable(void) { }

static inline void timer_arch_set(timer_value_t value) { }

static inline timer_value_t timer_arch_get_count(void)
{
    // return (timer_value_t)systick_get_cvr(systick_s);
    return 0;
}

static inline void timer_arch_init(void)
{
    timer_arch_disable();
}

#endif /* TIMER_ARCH_H */
