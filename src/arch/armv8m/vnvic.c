#include <arch/vnvic.h>
#include <arch/nvic.h>
#include <bitmap.h>
void vnvic_init(void) { }

void vnvic_reset(void) { }

static inline void vnvic_save_prio(irqid_t int_id, uint8_t* int_prio)
{
    int_prio[int_id] = (uint8_t)nvic_get_prio(nvic_ns, int_id);
}

static inline void vnvic_restore_prio(irqid_t int_id, uint8_t int_prio)
{
    nvic_set_prio(nvic_ns, int_id, int_prio);
}

static void vnvic_save_interrupt(irqid_t int_id, struct vnvic* vnvic)
{
    // Read int enable status and store in bitmap
    if (nvic_get_en(nvic_ns, int_id)) {
        bitmap_set(vnvic->int_enab, int_id);
        // Deactivate int to prevent triggering of int inside bao
        // TODO:ARMV8M - We need to test if by disabling the interrupt, the irq still gets pended if
        // triggered
        nvic_enable(nvic_ns, int_id, false);
    } else {
        bitmap_clear(vnvic->int_enab, int_id);
    }

    // Read int pending status and store in bitmap
    if (nvic_get_pend(nvic_ns, int_id)) {
        bitmap_set(vnvic->int_pend, int_id);
    } else {
        bitmap_clear(vnvic->int_pend, int_id);
    }

    // Save interrupt priority
    vnvic_save_prio(int_id, vnvic->int_prio);

    nvic_int_target(SECURE, int_id);
}

static void vnvic_restore_interrupt(irqid_t int_id, bool en, bool pend, uint8_t prio)
{
    nvic_int_target(NONSECURE, int_id);

    if (en) {
        nvic_enable(nvic_ns, int_id, true);
    }
    if (pend) {
        nvic_set_pend(nvic_ns, int_id);
    }

    // Restore interrupt priority
    vnvic_restore_prio(int_id, prio);
}

void vnvic_save_state(struct vnvic* vnvic, bitmap_t *int_bitmap)
{
    // TODO-ARMV8M - This can be optimized
    for (irqid_t int_id = 0; int_id < MAX_INTERRUPTS; int_id++) {
        if (bitmap_get(int_bitmap, int_id)) {
            vnvic_save_interrupt(int_id, vnvic);
        }
    }
}

void vnvic_restore_state(struct vnvic* vnvic, bitmap_t *int_bitmap)
{
    // TODO-ARMV8M - This can be optimized
    for (irqid_t int_id = 0; int_id < MAX_INTERRUPTS; int_id++) {
        if (bitmap_get(int_bitmap, int_id)) {
            vnvic_restore_interrupt(int_id, bitmap_get(vnvic->int_enab, int_id),
                bitmap_get(vnvic->int_pend, int_id), vnvic->int_prio[int_id]);
        }
    }
}
