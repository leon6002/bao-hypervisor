#ifndef VNVIC_H
#define VNVIC_H

#include <bao.h>
#include <bitmap.h>
#include <arch/interrupts.h>

struct vnvic {
    BITMAP_ALLOC(int_enab, MAX_INTERRUPTS);
    BITMAP_ALLOC(int_pend, MAX_INTERRUPTS);

    uint8_t int_prio[MAX_INTERRUPTS]; // TODO:ARMV8-M - This should be allocated dynamically
};

struct vnvic;

void vnvic_init(void);
void vnvic_reset(void);
void vnvic_save_state(struct vnvic* vnvic, bitmap_t *int_bitmap);
void vnvic_restore_state(struct vnvic* vnvic, bitmap_t *int_bitmap);

#endif /* VNVIC_H */
