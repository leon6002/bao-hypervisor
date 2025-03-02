#include <arch/vmpu.h>
#include <vm.h>
#include <string.h>
#include <arch/sysregs.h>

typedef unsigned long region_t;

void vmpu_init(struct vmpu* vmpu)
{
    UNUSED_ARG(vmpu);
}

void vmpu_reset(struct vmpu* vmpu) { }

void vmpu_save_state(struct vmpu* vmpu) { }

void vmpu_restore_state(struct vmpu* vmpu) { }
