#ifndef INTC_H
#define INTC_H

#include <bao.h>
#include <vm.h>

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
