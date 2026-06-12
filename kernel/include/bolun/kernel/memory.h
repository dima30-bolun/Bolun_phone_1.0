#ifndef BOLUN_KERNEL_MEMORY_H
#define BOLUN_KERNEL_MEMORY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "bolun/boot/boot_info.h"

#ifdef __cplusplus
extern "C" {
#endif

uint64_t BolunCountUsablePages(const BolunMemoryRegion *regions, size_t count);
bool BolunMemoryRangeContains(uint64_t outer_start, uint64_t outer_end, uint64_t inner_start, uint64_t inner_end);

#ifdef __cplusplus
}
#endif

#endif
