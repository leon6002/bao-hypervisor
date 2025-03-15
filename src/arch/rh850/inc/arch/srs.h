/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __ARCH_SRS_H__
#define __ARCH_SRS_H__

#include <bao.h>

#ifndef __ASSEMBLER__
#ifndef GENERATING_DEFS

/* BASIC SYS REG */
#pragma inline_asm get_eipc
static unsigned long get_eipc(void)
{
    stsr 0, r10, 0
}

#pragma inline_asm set_eipc
static void set_eipc(unsigned long val)
{
    ldsr r6, 0, 0
}

#pragma inline_asm get_eipsw
static unsigned long get_eipsw(void)
{
    stsr 1, r10, 0
}

#pragma inline_asm set_eipsw
static void set_eipsw(unsigned long val)
{
    ldsr r6, 1, 0
}

#pragma inline_asm get_fepc
static unsigned long get_fepc(void)
{
    stsr 2, r10, 0
}

#pragma inline_asm set_fepc
static void set_fepc(unsigned long val)
{
    ldsr r6, 2, 0
}

#pragma inline_asm get_fepsw
static unsigned long get_fepsw(void)
{
    stsr 3, r10, 0
}

#pragma inline_asm set_fepsw
static void set_fepsw(unsigned long val)
{
    ldsr r6, 3, 0
}

#pragma inline_asm get_psw
static unsigned long get_psw(void)
{
    stsr 5, r10, 0
}

#pragma inline_asm set_psw
static void set_psw(unsigned long val)
{
    ldsr r6, 5, 0
}

#pragma inline_asm get_fpsr
static unsigned long get_fpsr(void)
{
    stsr 6, r10, 0
}

#pragma inline_asm set_fpsr
static void set_fpsr(unsigned long val)
{
    ldsr r6, 6, 0
}

#pragma inline_asm get_fpepc
static unsigned long get_fpepc(void)
{
    stsr 7, r10, 0
}

#pragma inline_asm set_fpepc
static void set_fpepc(unsigned long val)
{
    ldsr r6, 7, 0
}

#pragma inline_asm get_fpst
static unsigned long get_fpst(void)
{
    stsr 8, r10, 0
}

#pragma inline_asm set_fpst
static void set_fpst(unsigned long val)
{
    ldsr r6, 8, 0
}

#pragma inline_asm get_fpcc
static unsigned long get_fpcc(void)
{
    stsr 9, r10, 0
}

#pragma inline_asm set_fpcc
static void set_fpcc(unsigned long val)
{
    ldsr r6, 9, 0
}

#pragma inline_asm get_fpcfg
static unsigned long get_fpcfg(void)
{
    stsr 10, r10, 0
}

#pragma inline_asm set_fpcfg
static void set_fpcfg(unsigned long val)
{
    ldsr r6, 10, 0
}

#pragma inline_asm get_eiic
static unsigned long get_eiic(void)
{
    stsr 13, r10, 0
}

#pragma inline_asm set_eiic
static void set_eiic(unsigned long val)
{
    ldsr r6, 13, 0
}

#pragma inline_asm get_feic
static unsigned long get_feic(void)
{
    stsr 14, r10, 0
}

#pragma inline_asm set_feic
static void set_feic(unsigned long val)
{
    ldsr r6, 14, 0
}

#pragma inline_asm get_ctpc
static unsigned long get_ctpc(void)
{
    stsr 16, r10, 0
}

#pragma inline_asm set_ctpc
static void set_ctpc(unsigned long val)
{
    ldsr r6, 16, 0
}

#pragma inline_asm get_ctpsw
static unsigned long get_ctpsw(void)
{
    stsr 17, r10, 0
}

#pragma inline_asm set_ctpsw
static void set_ctpsw(unsigned long val)
{
    ldsr r6, 17, 0
}

#pragma inline_asm get_ctbp
static unsigned long get_ctbp(void)
{
    stsr 20, r10, 0
}

#pragma inline_asm set_ctbp
static void set_ctbp(unsigned long val)
{
    ldsr r6, 20, 0
}

#pragma inline_asm get_snzcfg
static unsigned long get_snzcfg(void)
{
    stsr 21, r10, 0
}

#pragma inline_asm set_snzcfg
static void set_snzcfg(unsigned long val)
{
    ldsr r6, 21, 0
}

#pragma inline_asm get_eiwr
static unsigned long get_eiwr(void)
{
    stsr 28, r10, 0
}

#pragma inline_asm set_eiwr
static void set_eiwr(unsigned long val)
{
    ldsr r6, 28, 0
}

#pragma inline_asm get_fewr
static unsigned long get_fewr(void)
{
    stsr 29, r10, 0
}

#pragma inline_asm set_fewr
static void set_fewr(unsigned long val)
{
    ldsr r6, 29, 0
}

#pragma inline_asm get_spid
static unsigned long get_spid(void)
{
    stsr 0, r10, 1
}

#pragma inline_asm set_spid
static void set_spid(unsigned long val)
{
    ldsr r6, 0, 1
}

#pragma inline_asm get_spidlist
static unsigned long get_spidlist(void)
{
    stsr 1, r10, 1
}

#pragma inline_asm set_spidlist
static void set_spidlist(unsigned long val)
{
    ldsr r6, 1, 1
}

#pragma inline_asm get_rbase
static unsigned long get_rbase(void)
{
    stsr 2, r10, 1
}

#pragma inline_asm set_rbase
static void set_rbase(unsigned long val)
{
    ldsr r6, 2, 1
}

#pragma inline_asm get_ebase
static unsigned long get_ebase(void)
{
    stsr 3, r10, 1
}

#pragma inline_asm set_ebase
static void set_ebase(unsigned long val)
{
    ldsr r6, 3, 1
}

#pragma inline_asm get_peid
static unsigned long get_peid(void)
{
    stsr 0, r10, 2
}

#pragma inline_asm set_peid
static void set_peid(unsigned long val)
{
    ldsr r6, 0, 2
}

#pragma inline_asm get_bmid
static unsigned long get_bmid(void)
{
    stsr 1, r10, 2
}

#pragma inline_asm set_bmid
static void set_bmid(unsigned long val)
{
    ldsr r6, 1, 2
}

#pragma inline_asm get_intbp
static unsigned long get_intbp(void)
{
    stsr 4, r10, 1;
}

#pragma inline_asm set_intbp
static void set_intbp(unsigned long val)
{
    ldsr r6, 4, 1;
}

#pragma inline_asm get_mea
static unsigned long get_mea(void)
{
    stsr 6, r10, 2;
}

#pragma inline_asm set_mea
static void set_mea(unsigned long val)
{
    ldsr r6, 6, 2;
}

#pragma inline_asm get_mei
static unsigned long get_mei(void)
{
    stsr 8, r10, 2;
}

#pragma inline_asm set_mei
static void set_mei(unsigned long val)
{
    ldsr r6, 8, 2;
}

#pragma inline_asm get_rbip
static unsigned long get_rbip(void)
{
    stsr 18, r10, 2;
}

#pragma inline_asm set_rbip
static void set_rbip(unsigned long val)
{
    ldsr r6, 18, 2;
}

/* INTERRUPT SYS REGS */
#pragma inline_asm get_ispr
static unsigned long get_ispr(void)
{
    stsr 10, r10, 2
}

#pragma inline_asm set_ispr
static void set_ispr(unsigned long val)
{
    ldsr r6, 10, 2
}

#pragma inline_asm get_imsr
static unsigned long get_imsr(void)
{
    stsr 11, r10, 2
}

#pragma inline_asm set_imsr
static void set_imsr(unsigned long val)
{
    ldsr r6, 11, 2
}

#pragma inline_asm get_icsr
static unsigned long get_icsr(void)
{
    stsr 12, r10, 2
}

#pragma inline_asm set_icsr
static void set_icsr(unsigned long val)
{
    ldsr r6, 12, 2
}

#pragma inline_asm get_intcfg
static unsigned long get_intcfg(void)
{
    stsr 13, r10, 2
}

#pragma inline_asm set_intcfg
static void set_intcfg(unsigned long val)
{
    ldsr r6, 13, 2
}

#pragma inline_asm get_plmr
static unsigned long get_plmr(void)
{
    stsr 14, r10, 2
}

#pragma inline_asm set_plmr
static void set_plmr(unsigned long val)
{
    ldsr r6, 14, 2
}

/* MPU FUNCTION REGISTERS */
#pragma inline_asm get_mpm
static unsigned long get_mpm(void)
{
    stsr 0, r10, 5
}

#pragma inline_asm set_mpm
static void set_mpm(unsigned long val)
{
    ldsr r6, 0, 5
}

#pragma inline_asm get_mpcfg
static unsigned long get_mpcfg(void)
{
    stsr 2, r10, 5
}

#pragma inline_asm set_mpcfg
static void set_mpcfg(unsigned long val)
{
    ldsr r6, 2, 5
}

#pragma inline_asm get_mca
static unsigned long get_mca(void)
{
    stsr 8, r10, 5
}

#pragma inline_asm set_mca
static void set_mca(unsigned long val)
{
    ldsr r6, 8, 5
}

#pragma inline_asm get_mcs
static unsigned long get_mcs(void)
{
    stsr 9, r10, 5
}

#pragma inline_asm set_mcs
static void set_mcs(unsigned long val)
{
    ldsr r6, 9, 5
}

#pragma inline_asm get_mcc
static unsigned long get_mcc(void)
{
    stsr 10, r10, 5
}

#pragma inline_asm set_mcc
static void set_mcc(unsigned long val)
{
    ldsr r6, 10, 5
}

#pragma inline_asm get_mcr
static unsigned long get_mcr(void)
{
    stsr 11, r10, 5
}

#pragma inline_asm set_mcr
static void set_mcr(unsigned long val)
{
    ldsr r6, 11, 5
}

#pragma inline_asm get_mci
static unsigned long get_mci(void)
{
    stsr 12, r10, 5
}

#pragma inline_asm set_mci
static void set_mci(unsigned long val)
{
    ldsr r6, 12, 5
}

#pragma inline_asm get_mpidx
static unsigned long get_mpidx(void)
{
    stsr 16, r10, 5
}

#pragma inline_asm set_mpidx
static void set_mpidx(unsigned long val)
{
    ldsr r6, 16, 5
}

#pragma inline_asm get_mpbk
static unsigned long get_mpbk(void)
{
    stsr 17, r10, 5
}

#pragma inline_asm set_mpbk
static void set_mpbk(unsigned long val)
{
    ldsr r6, 17, 5
}

#pragma inline_asm get_mpla
static unsigned long get_mpla(void)
{
    stsr 20, r10, 5
}

#pragma inline_asm set_mpla
static void set_mpla(unsigned long val)
{
    ldsr r6, 20, 5
}

#pragma inline_asm get_mpua
static unsigned long get_mpua(void)
{
    stsr 21, r10, 5
}

#pragma inline_asm set_mpua
static void set_mpua(unsigned long val)
{
    ldsr r6, 21, 5
}

#pragma inline_asm get_mpat
static unsigned long get_mpat(void)
{
    stsr 22, r10, 5
}

#pragma inline_asm set_mpat
static void set_mpat(unsigned long val)
{
    ldsr r6, 22, 5
}

#pragma inline_asm get_mpid0
static unsigned long get_mpid0(void)
{
    stsr 24, r10, 5
}

#pragma inline_asm set_mpid0
static void set_mpid0(unsigned long val)
{
    ldsr r6, 24, 5
}

#pragma inline_asm get_mpid1
static unsigned long get_mpid1(void)
{
    stsr 25, r10, 5
}

#pragma inline_asm set_mpid1
static void set_mpid1(unsigned long val)
{
    ldsr r6, 25, 5
}

#pragma inline_asm get_mpid2
static unsigned long get_mpid2(void)
{
    stsr 26, r10, 5
}

#pragma inline_asm set_mpid2
static void set_mpid2(unsigned long val)
{
    ldsr r6, 26, 5
}

#pragma inline_asm get_mpid3
static unsigned long get_mpid3(void)
{
    stsr 27, r10, 5
}

#pragma inline_asm set_mpid3
static void set_mpid3(unsigned long val)
{
    ldsr r6, 27, 5
}

#pragma inline_asm get_mpid4
static unsigned long get_mpid4(void)
{
    stsr 28, r10, 5
}

#pragma inline_asm set_mpid4
static void set_mpid4(unsigned long val)
{
    ldsr r6, 28, 5
}

#pragma inline_asm get_mpid5
static unsigned long get_mpid5(void)
{
    stsr 29, r10, 5
}

#pragma inline_asm set_mpid5
static void set_mpid5(unsigned long val)
{
    ldsr r6, 29, 5
}

#pragma inline_asm get_mpid6
static unsigned long get_mpid6(void)
{
    stsr 30, r10, 5
}

#pragma inline_asm set_mpid6
static void set_mpid6(unsigned long val)
{
    ldsr r6, 30, 5
}

#pragma inline_asm get_mpid7
static unsigned long get_mpid7(void)
{
    stsr 31, r10, 5
}

#pragma inline_asm set_mpid7
static void set_mpid7(unsigned long val)
{
    ldsr r6, 31, 5
}

/* CACHE OPERAATION REGISTERS */
#pragma inline_asm get_ictagl
static unsigned long get_ictagl(void)
{
    stsr 16, r10, 4
}

#pragma inline_asm set_ictagl
static void set_ictagl(unsigned long val)
{
    ldsr r6, 16, 4
}

#pragma inline_asm get_ictagh
static unsigned long get_ictagh(void)
{
    stsr 17, r10, 4
}

#pragma inline_asm set_ictagh
static void set_ictagh(unsigned long val)
{
    ldsr r6, 17, 4
}

#pragma inline_asm get_icdatl
static unsigned long get_icdatl(void)
{
    stsr 18, r10, 4
}

#pragma inline_asm set_icdatl
static void set_icdatl(unsigned long val)
{
    ldsr r6, 18, 4
}

#pragma inline_asm get_icdath
static unsigned long get_icdath(void)
{
    stsr 19, r10, 4
}

#pragma inline_asm set_icdath
static void set_icdath(unsigned long val)
{
    ldsr r6, 19, 4
}

#pragma inline_asm get_icctrl
static unsigned long get_icctrl(void)
{
    stsr 24, r10, 4
}

#pragma inline_asm set_icctrl
static void set_icctrl(unsigned long val)
{
    ldsr r6, 24, 4
}

#pragma inline_asm get_iccfg
static unsigned long get_iccfg(void)
{
    stsr 26, r10, 4
}

#pragma inline_asm set_iccfg
static void set_iccfg(unsigned long val)
{
    ldsr r6, 26, 4
}

#pragma inline_asm get_icerr
static unsigned long get_icerr(void)
{
    stsr 28, r10, 4
}

#pragma inline_asm set_icerr
static void set_icerr(unsigned long val)
{
    ldsr r6, 28, 4
}

/* VIRTUALIZATION SUPPORT REGISTERS */
#pragma inline_asm get_hvcfg
static unsigned long get_hvcfg(void)
{
    stsr 16, r10, 1
}

#pragma inline_asm set_hvcfg
static void set_hvcfg(unsigned long val)
{
    ldsr r6, 16, 1
}

#pragma inline_asm get_gmcfg
static unsigned long get_gmcfg(void)
{
    stsr 17, r10, 1
}

#pragma inline_asm set_gmcfg
static void set_gmcfg(unsigned long val)
{
    ldsr r6, 17, 1
}

#pragma inline_asm get_pswh
static unsigned long get_pswh(void)
{
    stsr 15, r10, 0
}

#pragma inline_asm set_pswh
static void set_pswh(unsigned long val)
{
    ldsr r6, 15, 0
}

#pragma inline_asm get_eipswh
static unsigned long get_eipswh(void)
{
    stsr 18, r10, 0
}

#pragma inline_asm set_eipswh
static void set_eipswh(unsigned long val)
{
    ldsr r6, 18, 0
}

#pragma inline_asm get_fepswh
static unsigned long get_fepswh(void)
{
    stsr 19, r10, 0
}

#pragma inline_asm set_fepswh
static void set_fepswh(unsigned long val)
{
    ldsr r6, 19, 0
}

#pragma inline_asm get_hvsb
static unsigned long get_hvsb(void)
{
    stsr 20, r10, 1
}

#pragma inline_asm set_hvsb
static void set_hvsb(unsigned long val)
{
    ldsr r6, 20, 1
}

#pragma inline_asm get_dbgen
static unsigned long get_dbgen(void)
{
    stsr 0, r10, 3
}

#pragma inline_asm set_dbgen
static void set_dbgen(unsigned long val)
{
    ldsr r6, 0, 3
}

#pragma inline_asm get_gmpeid
static unsigned long get_gmpeid(void)
{
    stsr 30, r10, 9
}

#pragma inline_asm set_gmpeid
static void set_gmpeid(unsigned long val)
{
    ldsr r6, 30, 9
}

#pragma inline_asm get_gmspid
static unsigned long get_gmspid(void)
{
    stsr 16, r10, 9
}

#pragma inline_asm set_gmspid
static void set_gmspid(unsigned long val)
{
    ldsr r6, 16, 9
}

#endif /* GENERATING_DEFS */
#endif /* __ASSEMBLER__ */

#endif /* __ARCH_SRS_H__ */
