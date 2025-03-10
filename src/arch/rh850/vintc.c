#include <vm.h>
#include <platform.h>
#include <arch/intc.h>

bool vintc1_emul_handler(struct emul_access* acc)
{
    ERROR("%s not implemented", __func__);
}

bool vintc2_emul_handler(struct emul_access* acc)
{
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

    /* can we bypass intc1? */
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
