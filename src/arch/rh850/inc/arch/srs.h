/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __ARCH_SRS_H__
#define __ARCH_SRS_H__

#include <bao.h>

#ifndef __ASSEMBLER__

#define CSFRS_GEN_ACCESSORS_NAMED(csfr_name, csfr_id)         \
    static inline unsigned long csfr_##csfr_name##_read(void) \
    {                                                         \
        unsigned long csfr_value;                             \
        return csfr_value;                                    \
    }                                                         \
    static inline void csfr_##csfr_name##_write(unsigned long csfr_value) { }

#endif

#endif /* __ARCH_SRS_H__ */
