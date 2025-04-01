#include <vm.h>
#include <interrupts.h>

void vintc_init(struct vcpu* vcpu);
void vintc_inject(struct vcpu* vcpu, irqid_t int_id);
void vbootctrl_init(struct vcpu* vcpu);
