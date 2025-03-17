#include <vm.h>
#include <arch/vm.h>
#include <platform.h>
#include <arch/intc.h>
#include <emul.h>
#include <bitmap.h>
#include <interrupts.h>

extern volatile struct intc1* intc1_hw;
extern volatile struct intc2* intc2_hw;
extern volatile struct intif* intif_hw;
extern volatile struct eint* eint_hw;
extern volatile struct fenc* fenc_hw;
extern volatile struct feinc* feinc_hw[PLAT_CPU_NUM];


void vintc_inject(struct vcpu* vcpu, irqid_t int_id)
{
    struct vm* vm = vcpu->vm;

    if (!vm_has_interrupt(vm, int_id)) {
        ERROR("VM tried to access unassigned interrupt");
    }
    intc_set_pend(int_id, true);
}

/* static void emulate_intc1_eic_access(struct emul_access *acc, size_t offset) */
/* { */
/*     size_t int_id = ALIGN(offset, 16)/16; */

/*     struct vcpu* vcpu = cpu()->vcpu; */
/*     struct vm* vm = vcpu->vm; */

/*     if(!vm_has_interrupt(vm, int_id)){ */
/*         ERROR("VM tried to access unassigned interrupt"); */
/*     } */

/*     if(acc->write){ */
/*         unsigned long val = vcpu_readreg(vcpu, acc->reg); */
/*         intc1_hw->EIC[int_id] = val; */
/* #warning "Unimplemented need to virtualize intc1 emulation peid" */
/*     } else { */
/*         unsigned int val = intc1_hw->EIC[int_id]; */
/*         vcpu_writereg(vcpu, acc->reg, val); */
/* #warning "Unimplemented need to virtualize intc1 emulation peid" */
/*     } */
/* } */

/* static void emulate_intc1_imr_access(struct emul_access *acc, size_t offset) */
/* { */
/*     struct vcpu* vcpu = cpu()->vcpu; */
/*     struct vm* vm = vcpu->vm; */

/*     if(acc->write){ */
/*         unsigned long val = vcpu_readreg(vcpu, acc->reg); */

/*         for(unsigned int i = 0; i < 32; i++){ */
/*             if(!vm_has_interrupt(vm, i)){ */
/*                 continue; */
/*             } */
/*             unsigned int imr_bit = (i % 32); */
/*             if((1UL << imr_bit) & val){ */
/*                 intc1_hw->IMR |= 1UL << imr_bit; */
/*             } */
/*         } */
/*     } else { */
/*         unsigned long val = 0; */

/*         for(unsigned int i = 0; i < 32; i++){ */
/*             if(!vm_has_interrupt(vm, i)){ */
/*                 continue; */
/*             } */
/*             unsigned int imr_bit = (i % 32); */
/*             unsigned int imr_val = intc1_hw->IMR; */
/*             if((1UL << imr_bit) & imr_val){ */
/*                 val |= (1UL << imr_bit); */
/*             } */
/*         } */
/*         vcpu_writereg(vcpu, acc->reg, val); */
/*     } */
/* } */

/* static void emulate_intc1_eeic_access(struct emul_access *acc, size_t offset) */
/* { */
/*     size_t int_id = ALIGN(offset, 32)/32; */

/*     struct vcpu* vcpu = cpu()->vcpu; */
/*     struct vm* vm = vcpu->vm; */

/*     if(!vm_has_interrupt(vm, int_id)){ */
/*         ERROR("VM tried to access unassigned interrupt"); */
/*     } */

/*     if(acc->write){ */
/*         unsigned long val = vcpu_readreg(vcpu, acc->reg); */
/*         intc1_hw->EEIC[int_id] = val; */
/* #warning "Unimplemented need to virtualize intc1 emulation peid" */
/*     } else { */
/*         unsigned int val = intc1_hw->EEIC[int_id]; */
/*         vcpu_writereg(vcpu, acc->reg, val); */
/* #warning "Unimplemented need to virtualize intc1 emulation peid" */
/*     } */
/* } */

/* static void emulate_intc1_eibg_access(struct emul_access *acc, size_t offset) */
/* { */
/*     struct vcpu* vcpu = cpu()->vcpu; */

/*     /1* TODO  Check priority threshold ... *1/ */

/*     if(acc->write){ */
/*         unsigned long val = vcpu_readreg(vcpu, acc->reg); */
/*         intc1_hw->EIBG = val; */
/*     } else { */
/*         /1* TODO access imr *1/ */
/*         unsigned int val = intc1_hw->EIBG; */
/*         vcpu_writereg(vcpu, acc->reg, val); */
/*     } */
/* } */

/* static bool vintc1_emul_handler(struct emul_access* acc) */
/* { */
/*     size_t acc_offset = acc->addr - platform.arch.intc.intc1_addr; */

/*     size_t intc1_eic_bot = offsetof(struct intc1, EIC); */
/*     size_t intc1_eic_top = sizeof(((struct intc1*)NULL)->EIC); */
/*     if(acc_offset >= intc1_eic_bot && acc_offset < intc1_eic_top){ */
/*         emulate_intc1_eic_access(acc, acc_offset - intc1_eic_bot); */
/*     } */

/*     size_t intc1_imr_bot = offsetof(struct intc1, IMR); */
/*     size_t intc1_imr_top = sizeof(((struct intc1*)NULL)->IMR); */
/*     if(acc_offset >= intc1_imr_bot && acc_offset < intc1_imr_top){ */
/*         emulate_intc1_imr_access(acc, acc_offset - intc1_imr_bot); */
/*     } */

/*     size_t intc1_eeic_bot = offsetof(struct intc1, EEIC); */
/*     size_t intc1_eeic_top = sizeof(((struct intc1*)NULL)->EEIC); */
/*     if(acc_offset >= intc1_eeic_bot && acc_offset < intc1_eeic_top){ */
/*         emulate_intc1_eeic_access(acc, acc_offset - intc1_eeic_bot); */
/*     } */

/*     size_t intc1_eibg_bot = offsetof(struct intc1, EIBG); */
/*     size_t intc1_eibg_top = sizeof(((struct intc1*)NULL)->EIBG); */
/*     if(acc_offset >= intc1_eibg_bot && acc_offset < intc1_eibg_top){ */
/*         emulate_intc1_eibg_access(acc, acc_offset - intc1_eibg_bot); */
/*     } */

/*     ERROR("%s not implemented", __func__); */
/* } */

static void emulate_intc2_eic_access(struct emul_access* acc, size_t offset)
{
    size_t eeic_idx = ALIGN(offset, 16) / 16;
    irqid_t int_id = eeic_idx + 32;

    struct vcpu* vcpu = cpu()->vcpu;
    struct vm* vm = vcpu->vm;

    if (!vm_has_interrupt(vm, int_id)) {
        ERROR("VM tried to access unassigned interrupt");
    }

    if (acc->write) {
        unsigned long val = vcpu_readreg(vcpu, acc->reg);
        intc2_hw->EIC[eeic_idx] = val;
    } else {
        unsigned int val = intc2_hw->EIC[eeic_idx];
        vcpu_writereg(vcpu, acc->reg, val);
    }
}

static void emulate_intc2_imr_access(struct emul_access* acc, size_t offset)
{
    struct vcpu* vcpu = cpu()->vcpu;
    struct vm* vm = vcpu->vm;

    size_t imr_idx = ALIGN(offset, 32) / 32;

    /* code below is a bit messy but it should work since we are multiple of 32bit*/
    irqid_t first_imr_int = imr_idx * 32 + 32;
    if (acc->write) {
        unsigned long val = vcpu_readreg(vcpu, acc->reg);

        for (unsigned int i = first_imr_int; i < first_imr_int + 32; i++) {
            if (!vm_has_interrupt(vm, i)) {
                continue;
            }
            unsigned int imr_bit = (i % 32);
            if ((1UL << imr_bit) & val) {
                intc2_hw->IMR[imr_idx] |= 1UL << imr_bit;
            }
        }
    } else {
        unsigned long val = 0;

        for (unsigned int i = first_imr_int; i < first_imr_int + 32; i++) {
            if (!vm_has_interrupt(vm, i)) {
                continue;
            }
            unsigned int imr_bit = (i % 32);
            unsigned int imr_val = intc2_hw->IMR[imr_idx];
            if ((1UL << imr_bit) & imr_val) {
                val |= 1UL << imr_bit;
            }
        }
        vcpu_writereg(vcpu, acc->reg, val);
    }
}

static void emulate_intc2_eibd_access(struct emul_access* acc, size_t offset)
{
    size_t eibd_idx = ALIGN(offset, 32) / 32;
    irqid_t int_id = eibd_idx + 32;

    struct vcpu* vcpu = cpu()->vcpu;
    struct vm* vm = vcpu->vm;

    if (!vm_has_interrupt(vm, int_id)) {
        ERROR("VM tried to access unassigned interrupt");
    }

    /* we use 0xFFFF0000 to mask access to virtualization configuration */
    if (acc->write) {
        unsigned long val = vcpu_readreg(vcpu, acc->reg);
        intc2_hw->EIBD[eibd_idx] = val & 0xFFFF0000;
    } else {
        unsigned int val = intc2_hw->EIBD[eibd_idx] & 0xFFFF0000;
        vcpu_writereg(vcpu, acc->reg, val);
    }
}

static void emulate_intc2_eeic_access(struct emul_access* acc, size_t offset)
{
    size_t eeic_idx = ALIGN(offset, 32) / 32;
    size_t int_id = eeic_idx + 32;

    struct vcpu* vcpu = cpu()->vcpu;
    struct vm* vm = vcpu->vm;

    if (!vm_has_interrupt(vm, int_id)) {
        ERROR("VM tried to access unassigned interrupt");
    }

    if (acc->write) {
        unsigned long val = vcpu_readreg(vcpu, acc->reg);
        intc2_hw->EEIC[eeic_idx] = val;
#warning "Unimplemented need to virtualize intc2 emulation peid"
    } else {
        unsigned int val = intc2_hw->EEIC[eeic_idx];
        vcpu_writereg(vcpu, acc->reg, val);
#warning "Unimplemented need to virtualize intc2 emulation peid"
    }
}

bool vintc2_emul_handler(struct emul_access* acc)
{
    size_t acc_offset = acc->addr - platform.arch.intc.intc2_addr;

    size_t intc2_eic_bot = offsetof(struct intc2, EIC);
    size_t intc2_eic_top = sizeof(((struct intc2*)NULL)->EIC);
    if (acc_offset >= intc2_eic_bot && acc_offset < intc2_eic_top) {
        emulate_intc2_eic_access(acc, acc_offset - intc2_eic_bot);
    }

    size_t intc2_imr_bot = offsetof(struct intc2, IMR);
    size_t intc2_imr_top = sizeof(((struct intc2*)NULL)->IMR);
    if (acc_offset >= intc2_imr_bot && acc_offset < intc2_imr_top) {
        emulate_intc2_imr_access(acc, acc_offset - intc2_imr_bot);
    }

    size_t intc2_eibd_bot = offsetof(struct intc2, EIBD);
    size_t intc2_eibd_top = sizeof(((struct intc2*)NULL)->EIBD);
    if (acc_offset >= intc2_eibd_bot && acc_offset < intc2_eibd_top) {
        emulate_intc2_eibd_access(acc, acc_offset - intc2_eibd_bot);
    }

    size_t intc2_eeic_bot = offsetof(struct intc2, EEIC);
    size_t intc2_eeic_top = sizeof(((struct intc2*)NULL)->EEIC);
    if (acc_offset >= intc2_eeic_bot && acc_offset < intc2_eeic_top) {
        emulate_intc2_eeic_access(acc, acc_offset - intc2_eeic_bot);
    }

    ERROR("%s not implemented", __func__);
}

bool vintif_emul_handler(struct emul_access* acc)
{
    ERROR("%s not implemented", __func__);
}

bool veint_emul_handler(struct emul_access* acc)
{
    ERROR("%s not implemented", __func__);
}

bool vfenc_emul_handler(struct emul_access* acc)
{
    ERROR("%s not implemented", __func__);
}

bool vfeinc_emul_handler(struct emul_access* acc)
{
    ERROR("%s not implemented", __func__);
}

void vintc_init(struct vcpu* vcpu)
{
    struct vm* vm = vcpu->vm;

    /* vm->arch.intc1_emul = (struct emul_mem){ */
    /*     .va_base = platform.arch.intc.intc1_addr, */
    /*     .size = ALIGN(sizeof(struct intc1), PAGE_SIZE), */
    /*     .handler = vintc1_emul_handler, */
    /* }; */
    /* vm_emul_add_mem(vm, &vm->arch.intc1_emul); */

    vm->arch.intc2_emul = (struct emul_mem){
        .va_base = platform.arch.intc.intc2_addr,
        .size = ALIGN(sizeof(struct intc2), PAGE_SIZE),
        .handler = vintc2_emul_handler,
    };
    vm_emul_add_mem(vm, &vm->arch.intc2_emul);

    /* of the following which can we bypass? */
    vm->arch.intif_emul = (struct emul_mem){
        .va_base = platform.arch.intc.intif_addr,
        .size = ALIGN(sizeof(struct intif), PAGE_SIZE),
        .handler = vintif_emul_handler,
    };
    vm_emul_add_mem(vm, &vm->arch.intif_emul);

    vm->arch.eint_emul = (struct emul_mem){
        .va_base = platform.arch.intc.eint_addr,
        .size = ALIGN(sizeof(struct eint), PAGE_SIZE),
        .handler = veint_emul_handler,
    };
    vm_emul_add_mem(vm, &vm->arch.eint_emul);

    vm->arch.fenc_emul = (struct emul_mem){
        .va_base = platform.arch.intc.fenc_addr,
        .size = ALIGN(sizeof(struct fenc), PAGE_SIZE),
        .handler = vfenc_emul_handler,
    };
    vm_emul_add_mem(vm, &vm->arch.fenc_emul);

    vm->arch.feinc_emul = (struct emul_mem){
        .va_base = platform.arch.intc.feinc_addr[cpu()->id],
        .size = ALIGN(sizeof(struct feinc), PAGE_SIZE),
        .handler = vfeinc_emul_handler,
    };
    vm_emul_add_mem(vm, &vm->arch.feinc_emul);
}
