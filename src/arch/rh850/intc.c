#include <bao.h>
#include <platform_defs.h>
#include <platform.h>
#include <interrupts.h>
#include <mem.h>
#include <cpu.h>

#define PRIVATE_IRQS_NUM 32

struct intc1 {
    uint16_t EIC[PRIVATE_IRQS_NUM]; /* 0-0x20*/
    uint32_t PAD0[88];              /* 0x20-0xf0 */
    uint32_t IMR;                   /* 0xf0 - 0xf4 */
    uint32_t PAD1[3];               /* 0xf4 - 0x100 */
    uint32_t EIBD[32];              /* 0x100 - 0x180 */
    uint32_t PAD3[16];              /* 0x180 - 0x1C0 */
    uint32_t FIBD;                  /* 0x1C0 0x1c4 */
    uint32_t PAD4[15];              /* 0x1C4 - 0x200 */
    uint32_t EEIC[32];              /* 0x200 - 0x280 */
    uint32_t PAD5[15];              /* 0x1C4 - 0x200 */
    uint32_t IHVCFG;                /* 0x200 - 0x204 */
    uint32_t PAD6[31];              /* 0x204 - 0x280 */
    uint32_t EIBG;                  /* 0x280 0x284 */
    uint32_t PAD7[15];              /* 0x284 - 0x2C0 */
    uint32_t FIBG;                  /* 0x2C0 - 0x2C4 */
};

#define INTC2_IRQ_NUM (MAX_INTERRUPT_LINES - PRIVATE_IRQS_NUM)
#define INTC2_IMR_NUM ((uint32_t)((MAX_INTERRUPT_LINES + 0.5) / 32) - 1)

/* TODO HOW TO be smart calculating padding? */
struct intc2 {
    uint16_t EIC[INTC2_IRQ_NUM];
    uint32_t PAD0[(0x1000 - (INTC2_IRQ_NUM * 2)) / 4]; /* 0x... - 0x1000*/
    uint32_t IMR[INTC2_IMR_NUM];                       /* 0x1000 - 0x... */
    uint32_t PAD1[3];                                  /*  */
    uint32_t I2EIBG[PLAT_CPU_NUM];                     /* 0x1FE0 */
    uint32_t PAD2[15];                                 /*  */
    uint32_t EIBD[INTC2_IRQ_NUM];                      /* 0x2000 -  */
    uint32_t PAD3[15];                                 /*  */
    uint32_t EEIC[INTC2_IRQ_NUM];                      /* 0x4000 -  */
};

struct intif {
    uint32_t PINT[8];
    uint32_t PINTCLR[8];
    uint32_t PAD[112];
    uint32_t TPTMSEL;
};

struct eint {
    uint32_t sintr[PLAT_CPU_NUM];
};

struct fenc {
    uint64_t FENMIF;
    uint32_t FENMIC;
};

struct feinc {
    uint32_t FEINTF;
    uint32_t FEINTFMSK;
    uint32_t FEINTC;
};

static volatile struct intc1* intc1_hw_pe[PLAT_CPU_NUM];
static volatile struct intc2* intc2_hw;
static volatile struct intif* intif_hw;
static volatile struct eint* eint_hw;
static volatile struct fenc* fenc_hw;
static volatile struct feinc* feinc_hw[PLAT_CPU_NUM];

// EIC Register Bit Definitions
#define EICTn_BIT                (1 << 15)
#define EIRFn_BIT                (1 << 12)
#define EIMKn_BIT                (1 << 7)
#define EITBn_BIT                (1 << 6)
#define EIOVn_BIT                (1 << 5)

// EIPn Mask (Bits 3-0)
#define EIPn_MASK                (0xF)

// Macro to Set/Reset EICTn
#define EIC_SET_EICTn(reg)       ((reg) |= EICTn_BIT)        // Set EICTn to 1 (Detection by level)
#define EIC_CLR_EICTn(reg)       ((reg) &= ~EICTn_BIT)       // Clear EICTn to 0 (Detection by edge)
#define EIC_GET_EICTn(reg)       (((reg) & EICTn_BIT) >> 15) // Read EICTn value

// Macro to Set/Reset EIRFn
#define EIC_SET_EIRFn(reg)       ((reg) |= EIRFn_BIT)  // Set EIRFn to 1 (Interrupt request present)
#define EIC_CLR_EIRFn(reg)       ((reg) &= ~EIRFn_BIT) // Clear EIRFn to 0 (No interrupt request)
#define EIC_GET_EIRFn(reg)       (((reg) & EIRFn_BIT) >> 12) // Read EIRFn value

// Macro to Set/Reset EIMKn
#define EIC_SET_EIMKn(reg)       ((reg) |= EIMKn_BIT)       // Mask interrupt
#define EIC_CLR_EIMKn(reg)       ((reg) &= ~EIMKn_BIT)      // Unmask interrupt
#define EIC_GET_EIMKn(reg)       (((reg) & EIMKn_BIT) >> 7) // Read EIMKn value

// Macro to Set/Reset EITBn
#define EIC_SET_EITBn(reg)       ((reg) |= EITBn_BIT)  // Set EITBn to 1 (Table reference method)
#define EIC_CLR_EITBn(reg)       ((reg) &= ~EITBn_BIT) // Set EITBn to 0 (Direct vector method)
#define EIC_GET_EITBn(reg)       (((reg) & EITBn_BIT) >> 6) // Read EITBn value

// Macro to Set/Reset EIOVn
#define EIC_SET_EIOVn(reg)       ((reg) |= EIOVn_BIT)       // Set EIOVn to 1 (Interrupt overflow)
#define EIC_CLR_EIOVn(reg)       ((reg) &= ~EIOVn_BIT)      // Clear EIOVn to 0 (No overflow)
#define EIC_GET_EIOVn(reg)       (((reg) & EIOVn_BIT) >> 5) // Read EIOVn value

// Macro to Set/Reset EIPn
#define EIC_SET_EIPn(reg, value) ((reg) = ((reg) & ~EIPn_MASK) | ((value) & EIPn_MASK))
#define EIC_GET_EIPn(reg)        ((reg) & EIPn_MASK)

// EIBD Register Bit Definitions
#define EIBD_GM_BIT              (1 << 15) // Guest Mode Bit

// GPID[2:0] Mask (Bits 10-8)
#define EIBD_GPID_MASK           (0x7 << 8) // 3-bit mask for GPID field
#define EIBD_GPID_SHIFT          8          // GPID starting position

// PEID[2:0] Mask (Bits 2-0)
#define EIBD_PEID_MASK           (0x7) // 3-bit mask for PEID field

// Macro to Set/Reset GM
#define EIBD_SET_GM(reg)         ((reg) |= EIBD_GM_BIT)  // Set GM to 1 (Channel bound to Guest)
#define EIBD_CLR_GM(reg)         ((reg) &= ~EIBD_GM_BIT) // Clear GM to 0 (Channel bound to Host)
#define EIBD_GET_GM(reg)         (((reg) & EIBD_GM_BIT) >> 15) // Read GM value

// Macro to Set GPID (Bits 10-8)
#define EIBD_SET_GPID(reg, value) \
    ((reg) = ((reg) & ~EIBD_GPID_MASK) | (((value) & 0x7) << EIBD_GPID_SHIFT))
#define EIBD_GET_GPID(reg)        (((reg) & EIBD_GPID_MASK) >> EIBD_GPID_SHIFT)

// Macro to Set PEID (Bits 2-0)
#define EIBD_SET_PEID(reg, value) ((reg) = ((reg) & ~EIBD_PEID_MASK) | ((value) & EIBD_PEID_MASK))
#define EIBD_GET_PEID(reg)        ((reg) & EIBD_PEID_MASK)

void intc_set_pend(irqid_t int_id, bool en)
{
    if (int_id < PRIVATE_IRQS_NUM) {
        if (en) {
            EIC_SET_EIRFn(intc1_hw_pe[cpu()->id]->EIC[int_id]);
        } else {
            EIC_CLR_EIRFn(intc1_hw_pe[cpu()->id]->EIC[int_id]);
        }
    } else {
        irqid_t intc2_irq_id = int_id - PRIVATE_IRQS_NUM;
        if (en) {
            EIC_SET_EIRFn(intc2_hw->EIC[intc2_irq_id]);
        } else {
            EIC_CLR_EIRFn(intc2_hw->EIC[intc2_irq_id]);
        }
    }
}

bool intc_get_pend(irqid_t int_id)
{
    unsigned int pend = 0;
    if (int_id < PRIVATE_IRQS_NUM) {
        pend = EIC_GET_EIRFn(intc1_hw_pe[cpu()->id]->EIC[int_id]);

    } else {
        irqid_t intc2_irq_id = int_id - PRIVATE_IRQS_NUM;
        pend = EIC_GET_EIRFn(intc2_hw->EIC[intc2_irq_id]);
    }

    return !!pend;
}

void intc_hyp_assign(irqid_t int_id)
{
    if (int_id < PRIVATE_IRQS_NUM) {
        EIBD_CLR_GM(intc1_hw_pe[cpu()->id]->EIBD[int_id]);
    } else {
        irqid_t intc2_irq_id = int_id - PRIVATE_IRQS_NUM;
        EIBD_CLR_GM(intc2_hw->EIBD[intc2_irq_id]);
    }
}

void intc_vm_assign(irqid_t int_id, vmid_t vm_id)
{
    if (int_id < PRIVATE_IRQS_NUM) {
        EIBD_SET_GM(intc1_hw_pe[cpu()->id]->EIBD[int_id]);
        EIBD_SET_GPID(intc1_hw_pe[cpu()->id]->EIBD[int_id], vm_id);
    } else {
        irqid_t intc2_irq_id = int_id - PRIVATE_IRQS_NUM;
        EIBD_SET_GM(intc2_hw->EIBD[intc2_irq_id]);
        EIBD_SET_GPID(intc2_hw->EIBD[intc2_irq_id], vm_id);

        /* assumes calling cpu is configuring this interrupt */
        EIBD_SET_PEID(intc2_hw->EIBD[intc2_irq_id], cpu()->id);
    }
}

void intc_set_trgt(irqid_t int_id, cpuid_t cpu_id)
{
    if (int_id < PRIVATE_IRQS_NUM) {
        if (cpu()->id != cpu_id) {
            ERROR("setting private interrupt on another core");
        }
    } else {
        irqid_t intc2_irq_id = int_id - PRIVATE_IRQS_NUM;
        EIBD_SET_PEID(intc2_hw->EIBD[intc2_irq_id], cpu_id);
    }
}

void intc_set_enable(irqid_t int_id, bool en)
{
    if (int_id < PRIVATE_IRQS_NUM) {
        if (en) {
            EIC_SET_EIMKn(intc1_hw_pe[cpu()->id]->EIC[int_id]);
            EIC_SET_EITBn(intc1_hw_pe[cpu()->id]->EIC[int_id]);
        } else {
            EIC_CLR_EIMKn(intc1_hw_pe[cpu()->id]->EIC[int_id]);
            EIC_CLR_EITBn(intc1_hw_pe[cpu()->id]->EIC[int_id]);
        }
    } else {
        irqid_t intc2_irq_id = int_id - PRIVATE_IRQS_NUM;
        if (en) {
            EIC_SET_EIMKn(intc2_hw->EIC[intc2_irq_id]);
            EIC_SET_EITBn(intc2_hw->EIC[intc2_irq_id]);
        } else {
            EIC_CLR_EIMKn(intc2_hw->EIC[intc2_irq_id]);
            EIC_CLR_EITBn(intc2_hw->EIC[intc2_irq_id]);
        }
    }
}

void intc_set_prio(irqid_t int_id, unsigned long prio)
{
    if (int_id < PRIVATE_IRQS_NUM) {
        EIC_SET_EIPn(intc1_hw_pe[cpu()->id]->EIC[int_id], prio);
    } else {
        irqid_t intc2_irq_id = int_id - PRIVATE_IRQS_NUM;
        EIC_SET_EIPn(intc2_hw->EIC[intc2_irq_id], prio);
    }
}

static void intc_map_local_mmio()
{
    vaddr_t inc1_ptr;
    /* because we are mapping an alias this could be global mapping actually */
    inc1_ptr = mem_alloc_map_dev(&cpu()->as, SEC_HYP_PRIVATE, INVALID_VA,
        platform.arch.intc.intc1_addr, NUM_PAGES(sizeof(struct intc1)));
    if (inc1_ptr == INVALID_VA) {
        ERROR("maping intc1 failed");
    }

    intc1_hw_pe[cpu()->id] = (struct intc1*)inc1_ptr;

    vaddr_t feinc_ptr;
    feinc_ptr = mem_alloc_map_dev(&cpu()->as, SEC_HYP_PRIVATE, INVALID_VA,
        platform.arch.intc.feinc_addr[cpu()->id], NUM_PAGES(sizeof(struct feinc)));
    if (feinc_ptr == INVALID_VA) {
        ERROR("maping feinc_ptr failed");
    }

    feinc_hw[cpu()->id] = (struct feinc*)feinc_ptr;
}

static void intc_map_global_mmio()
{
    vaddr_t global_start_addr = platform.arch.intc.intc2_addr;
    vaddr_t global_end_addr = platform.arch.intc.fenc_addr + sizeof(struct fenc);
    size_t global_size = global_end_addr - global_start_addr;
    size_t glabal_npages = NUM_PAGES(global_size);

    vaddr_t global_ptr;
    global_ptr =
        mem_alloc_map_dev(&cpu()->as, SEC_HYP_GLOBAL, INVALID_VA, global_start_addr, glabal_npages);
    if (global_ptr == INVALID_VA) {
        ERROR("maping global interrupt controller region failed");
    }

    intc2_hw = (void*)platform.arch.intc.intc2_addr;
    intif_hw = (void*)platform.arch.intc.intif_addr;
    eint_hw = (void*)platform.arch.intc.eint_addr;
    fenc_hw = (void*)platform.arch.intc.fenc_addr;
    feinc_hw[cpu()->id] = (void*)platform.arch.intc.feinc_addr[cpu()->id];
}

void intc_map_mmio()
{
    intc_map_local_mmio();
    if (cpu_is_master()) {
        intc_map_global_mmio();
    }
}

/* TODO needed? */
/* void intc_clk_init() */
/* { */
/* } */

void intc_init()
{
    intc_map_mmio();
}
