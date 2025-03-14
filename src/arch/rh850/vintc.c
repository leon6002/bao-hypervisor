#include <vm.h>
#include <arch/vm.h>
#include <platform.h>
#include <arch/intc.h>
#include <emul.h>

static void emulate_intc1_eic_access(struct emul_access *acc, size_t offset)
{

    size_t int_id = ALIGN(offset, 16)/16;

    struct vcpu* vcpu = cpu()->vcpu;
    struct vm* vm = vcpu->vm;

    if(!vm_has_interrupt(vm, int_id)){
        ERROR("VM tried to access unassigned interrupt");
    }

    if(acc->write){
        unsigned long val = vcpu_readreg(vcpu, acc->reg);
        /* TODO access eic */
    } else {
        /* TODO access eic */
        unsigned int val = 0;
        vcpu_writereg(vcpu, acc->reg, val);
    }
}

static void emulate_intc1_imr_access(struct emul_access *acc, size_t offset)
{
    struct vcpu* vcpu = cpu()->vcpu;
    struct vm* vm = vcpu->vm;

    for(unsigned int i = 0; i < 32; i++){
        if(!vm_has_interrupt(vm, i)){
            continue;
        }

        if(acc->write){
            unsigned long val = vcpu_readreg(vcpu, acc->reg);
            /* TODO access imr */
        } else {
            /* TODO access imr */
            unsigned int val = 0;
            vcpu_writereg(vcpu, acc->reg, val);
        }
    }
}

static void emulate_intc1_eibd_access(struct emul_access *acc, size_t offset)
{
    size_t int_id = ALIGN(offset, 32)/32;

    struct vcpu* vcpu = cpu()->vcpu;
    struct vm* vm = vcpu->vm;

    if(!vm_has_interrupt(vm, int_id)){
        ERROR("VM tried to access unassigned interrupt");
    }

    if(acc->write){
        unsigned long val = vcpu_readreg(vcpu, acc->reg);
        /* TODO access imr */
    } else {
        /* TODO access imr */
        unsigned int val = 0;
        vcpu_writereg(vcpu, acc->reg, val);
    }
}

static void emulate_intc1_fibd_access(struct emul_access *acc, size_t offset)
{
    struct vcpu* vcpu = cpu()->vcpu;

    /* how to check if guest can use FE? */

    if(acc->write){
        unsigned long val = vcpu_readreg(vcpu, acc->reg);
        /* TODO access imr */
    } else {
        /* TODO access imr */
        unsigned int val = 0;
        vcpu_writereg(vcpu, acc->reg, val);
    }
}

static void emulate_intc1_eeic_access(struct emul_access *acc, size_t offset)
{
    size_t int_id = ALIGN(offset, 32)/32;

    struct vcpu* vcpu = cpu()->vcpu;
    struct vm* vm = vcpu->vm;

    if(!vm_has_interrupt(vm, int_id)){
        ERROR("VM tried to access unassigned interrupt");
    }

    if(acc->write){
        unsigned long val = vcpu_readreg(vcpu, acc->reg);
        /* TODO access imr */
    } else {
        /* TODO access imr */
        unsigned int val = 0;
        vcpu_writereg(vcpu, acc->reg, val);
    }
}


static void emulate_intc1_eibg_access(struct emul_access *acc, size_t offset)
{
    struct vcpu* vcpu = cpu()->vcpu;

    /* TODO  Check priority threshold ... */

    if(acc->write){
        unsigned long val = vcpu_readreg(vcpu, acc->reg);
        /* TODO access imr */
    } else {
        /* TODO access imr */
        unsigned int val = 0;
        vcpu_writereg(vcpu, acc->reg, val);
    }
}


static void emulate_intc1_fibg_access(struct emul_access *acc, size_t offset)
{
    struct vcpu* vcpu = cpu()->vcpu;

    /* TODO */

    if(acc->write){
        unsigned long val = vcpu_readreg(vcpu, acc->reg);
        /* TODO access imr */
    } else {
        /* TODO access imr */
        unsigned int val = 0;
        vcpu_writereg(vcpu, acc->reg, val);
    }
}

static bool vintc1_emul_handler(struct emul_access* acc)
{
    size_t acc_offset = acc->addr - platform.arch.intc.intc1_addr;

    size_t intc1_eic_bot = offsetof(struct intc1, EIC);
    size_t intc1_eic_top = sizeof(((struct intc1*)NULL)->EIC);
    if(acc_offset >= intc1_eic_bot && acc_offset < intc1_eic_top){
        emulate_intc1_eic_access(acc, acc_offset - intc1_eic_bot);
    }

    size_t intc1_imr_bot = offsetof(struct intc1, IMR);
    size_t intc1_imr_top = sizeof(((struct intc1*)NULL)->IMR);
    if(acc_offset >= intc1_imr_bot && acc_offset < intc1_imr_top){
        emulate_intc1_imr_access(acc, acc_offset - intc1_imr_bot);
    }

    size_t intc1_eibd_bot = offsetof(struct intc1, EIBD);
    size_t intc1_eibd_top = sizeof(((struct intc1*)NULL)->EIBD);
    if(acc_offset >= intc1_eibd_bot && acc_offset < intc1_eibd_top){
        emulate_intc1_eibd_access(acc, acc_offset - intc1_eibd_bot);
    }

    size_t intc1_fibd_bot = offsetof(struct intc1, FIBD);
    size_t intc1_fibd_top = sizeof(((struct intc1*)NULL)->FIBD);
    if(acc_offset >= intc1_fibd_bot && acc_offset < intc1_fibd_top){
        emulate_intc1_fibd_access(acc, acc_offset - intc1_fibd_bot);
    }

    size_t intc1_eeic_bot = offsetof(struct intc1, EEIC);
    size_t intc1_eeic_top = sizeof(((struct intc1*)NULL)->EEIC);
    if(acc_offset >= intc1_eeic_bot && acc_offset < intc1_eeic_top){
        emulate_intc1_eeic_access(acc, acc_offset - intc1_eeic_bot);
    }

    size_t intc1_eibg_bot = offsetof(struct intc1, EIBG);
    size_t intc1_eibg_top = sizeof(((struct intc1*)NULL)->EIBG);
    if(acc_offset >= intc1_eibg_bot && acc_offset < intc1_eibg_top){
        emulate_intc1_eibg_access(acc, acc_offset - intc1_eibg_bot);
    }

    size_t intc1_fibg_bot = offsetof(struct intc1, FIBG);
    size_t intc1_fibg_top = sizeof(((struct intc1*)NULL)->FIBG);
    if(acc_offset >= intc1_fibg_bot && acc_offset < intc1_fibg_top){
        emulate_intc1_fibg_access(acc, acc_offset - intc1_fibg_bot);
    }


    ERROR("%s not implemented", __func__);
}



static void emulate_intc2_eic_access(struct emul_access *acc, size_t offset)
{
    size_t int_id = ALIGN(offset, 16)/16;

    struct vcpu* vcpu = cpu()->vcpu;
    struct vm* vm = vcpu->vm;

    if(!vm_has_interrupt(vm, int_id)){
        ERROR("VM tried to access unassigned interrupt");
    }

    if(acc->write){
        unsigned long val = vcpu_readreg(vcpu, acc->reg);
        /* TODO access eic */
    } else {
        /* TODO access eic */
        unsigned int val = 0;
        vcpu_writereg(vcpu, acc->reg, val);
    }
}

static void emulate_intc2_imr_access(struct emul_access *acc, size_t offset)
{
    size_t int_id = ALIGN(offset, 32)/32;

    struct vcpu* vcpu = cpu()->vcpu;
    struct vm* vm = vcpu->vm;

    if(!vm_has_interrupt(vm, int_id)){
        ERROR("VM tried to access unassigned interrupt");
    }

    if(acc->write){
        unsigned long val = vcpu_readreg(vcpu, acc->reg);
        /* TODO access imr */
    } else {
        /* TODO access imr */
        unsigned int val = 0;
        vcpu_writereg(vcpu, acc->reg, val);
    }
}

static void emulate_intc2_i2eibg_access(struct emul_access *acc, size_t offset)
{
    size_t int_id = ALIGN(offset, 32)/32;

    struct vcpu* vcpu = cpu()->vcpu;
    struct vm* vm = vcpu->vm;

    if(!vm_has_interrupt(vm, int_id)){
        ERROR("VM tried to access unassigned interrupt");
    }

    if(acc->write){
        unsigned long val = vcpu_readreg(vcpu, acc->reg);
        /* TODO access i2eibg */
    } else {
        /* TODO access i2eibg */
        unsigned int val = 0;
        vcpu_writereg(vcpu, acc->reg, val);
    }
}

static void emulate_intc2_eibd_access(struct emul_access *acc, size_t offset)
{
    size_t int_id = ALIGN(offset, 32)/32;

    struct vcpu* vcpu = cpu()->vcpu;
    struct vm* vm = vcpu->vm;

    if(!vm_has_interrupt(vm, int_id)){
        ERROR("VM tried to access unassigned interrupt");
    }

    if(acc->write){
        unsigned long val = vcpu_readreg(vcpu, acc->reg);
        /* TODO access eibd */
    } else {
        /* TODO access eibd */
        unsigned int val = 0;
        vcpu_writereg(vcpu, acc->reg, val);
    }
}

static void emulate_intc2_eeic_access(struct emul_access *acc, size_t offset)
{
    size_t int_id = ALIGN(offset, 32)/32;

    struct vcpu* vcpu = cpu()->vcpu;
    struct vm* vm = vcpu->vm;

    if(!vm_has_interrupt(vm, int_id)){
        ERROR("VM tried to access unassigned interrupt");
    }

    if(acc->write){
        unsigned long val = vcpu_readreg(vcpu, acc->reg);
        /* TODO access imr */
    } else {
        /* TODO access imr */
        unsigned int val = 0;
        vcpu_writereg(vcpu, acc->reg, val);
    }
}

bool vintc2_emul_handler(struct emul_access* acc)
{
    size_t acc_offset = acc->addr - platform.arch.intc.intc2_addr;

    size_t intc2_eic_bot = offsetof(struct intc2, EIC);
    size_t intc2_eic_top = sizeof(((struct intc2*)NULL)->EIC);
    if(acc_offset >= intc2_eic_bot && acc_offset < intc2_eic_top){
        emulate_intc2_eic_access(acc, acc_offset - intc2_eic_bot);
    }

    size_t intc2_imr_bot = offsetof(struct intc2, IMR);
    size_t intc2_imr_top = sizeof(((struct intc2*)NULL)->IMR);
    if(acc_offset >= intc2_imr_bot && acc_offset < intc2_imr_top){
        emulate_intc2_imr_access(acc, acc_offset - intc2_imr_bot);
    }

    size_t intc2_i2eibg_bot = offsetof(struct intc2, I2EIBG);
    size_t intc2_i2eibg_top = sizeof(((struct intc2*)NULL)->I2EIBG);
    if(acc_offset >= intc2_i2eibg_bot && acc_offset < intc2_i2eibg_top){
        emulate_intc2_i2eibg_access(acc, acc_offset - intc2_i2eibg_bot);
    }

    size_t intc2_eibd_bot = offsetof(struct intc2, EIBD);
    size_t intc2_eibd_top = sizeof(((struct intc2*)NULL)->EIBD);
    if(acc_offset >= intc2_eibd_bot && acc_offset < intc2_eibd_top){
        emulate_intc2_eibd_access(acc, acc_offset - intc2_eibd_bot);
    }

    size_t intc2_eeic_bot = offsetof(struct intc2, EEIC);
    size_t intc2_eeic_top = sizeof(((struct intc2*)NULL)->EEIC);
    if(acc_offset >= intc2_eeic_bot && acc_offset < intc2_eeic_top){
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

    vm->arch.intc1_emul = (struct emul_mem){
        .va_base = platform.arch.intc.intc1_addr,
        .size = ALIGN(sizeof(struct intc1), PAGE_SIZE),
        .handler = vintc1_emul_handler,
    };
    vm_emul_add_mem(vm, &vm->arch.intc1_emul);

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
