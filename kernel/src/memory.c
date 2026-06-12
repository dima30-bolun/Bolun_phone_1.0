#include "bolun/kernel/memory.h"

uint64_t BolunCountUsablePages(const BolunMemoryRegion *regions, size_t count)
{
    uint64_t pages = 0;

    if (regions == NULL) {
        return 0;
    }

    for (size_t index = 0; index < count; ++index) {
        if (regions[index].kind == BOLUN_MEMORY_USABLE && regions[index].end >= regions[index].start) {
            pages += (regions[index].end - regions[index].start) / BOLUN_PAGE_SIZE;
        }
    }

    return pages;
}

bool BolunMemoryRangeContains(uint64_t outer_start, uint64_t outer_end, uint64_t inner_start, uint64_t inner_end)
{
    return outer_start <= inner_start && inner_start <= inner_end && inner_end <= outer_end;
}
