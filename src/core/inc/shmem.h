/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef SHMEM_H
#define SHMEM_H

#include <mem.h>

struct shmem {
    size_t size;
    colormap_t colors;
    bool place_phys;
    /*
     * Holds the requested base before placement and the physical address after. This was an
     * anonymous union aliasing base/phys; it is a single field because CC-RH rejects
     * designated initializers for anonymous members and configs write it as .base.
     */
    paddr_t base;
    bool reserved;
    cpumap_t cpu_masters;
    spinlock_t lock;
};

void shmem_init(void);
struct shmem* shmem_get(size_t shmem_id);

#endif /* SHMEM_H */
