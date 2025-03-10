#ifndef INTC_H
#define INTC_H

#include <bao.h>
#include <vm.h>

void intc_init(void);

void intc_set_trgt(irqid_t int_id, cpuid_t cpu_id);

void intc_set_enable(irqid_t int_id, bool en);

void intc_set_prio(irqid_t int_id, unsigned long prio);

void intc_vm_assign(irqid_t int_id, vmid_t vm_id);
void intc_hyp_assign(irqid_t int_id);

void intc_set_act(irqid_t int_id, bool en);

void intc_set_pend(irqid_t int_id, bool en);
bool intc_get_pend(irqid_t int_id);
#endif /* INTC_H */
