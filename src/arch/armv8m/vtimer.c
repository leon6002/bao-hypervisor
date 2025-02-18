#include <arch/vtimer.h>
#include <vm.h>
#include <arch/sysregs.h>
#include <arch/systick.h>

void vtimer_init(struct vtimer* vtimer)
{
    UNUSED_ARG(vtimer);
}

void vtimer_reset(struct vtimer* vtimer)
{
    vtimer->csr = 0;
    vtimer->rvr = 0;
    vtimer->cvr = 0;
}

void vtimer_save_state(struct vtimer* vtimer)
{
    vtimer->csr = systick_get_csr(systick_ns);
    vtimer->rvr = systick_get_rvr(systick_ns);
    vtimer->cvr = systick_get_cvr(systick_ns);

    // TODO:ARMV8M - Save systick int state
    // vgic_hw_irq_save_state(vcpu, VTIMER_IRQ_ID);
    // gic_set_enable(VTIMER_IRQ_ID, false);
}

void vtimer_restore_state(struct vtimer* vtimer)
{
    systick_set_csr(systick_ns, vtimer->csr);
    systick_set_rvr(systick_ns, vtimer->rvr);
    systick_set_cvr(systick_ns, vtimer->cvr);

    // TODO:ARMV8M - Restore systick int state
    // vgic_hw_commit(vcpu, VTIMER_IRQ_ID);
}
