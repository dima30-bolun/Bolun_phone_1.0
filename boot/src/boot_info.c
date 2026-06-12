#include "bolun/boot/boot_info.h"

#include <stdbool.h>

static bool BolunIsSupportedCpu(BolunCpuArchitecture architecture)
{
    return architecture == BOLUN_CPU_ARCH_AARCH32 ||
           architecture == BOLUN_CPU_ARCH_AARCH64 ||
           architecture == BOLUN_CPU_ARCH_X86 ||
           architecture == BOLUN_CPU_ARCH_X86_64;
}

static bool BolunIsSupportedSoc(BolunSocFamily soc_family)
{
    switch (soc_family) {
    case BOLUN_SOC_QUALCOMM_SNAPDRAGON_S4:
    case BOLUN_SOC_QUALCOMM_SNAPDRAGON_200:
    case BOLUN_SOC_QUALCOMM_SNAPDRAGON_400:
    case BOLUN_SOC_QUALCOMM_SNAPDRAGON_800:
    case BOLUN_SOC_QUALCOMM_SNAPDRAGON_808:
    case BOLUN_SOC_QUALCOMM_SNAPDRAGON_810:
        return true;
    case BOLUN_SOC_UNKNOWN:
    default:
        return false;
    }
}

static bool BolunIsPageAligned(uint64_t address)
{
    return (address % BOLUN_PAGE_SIZE) == 0U;
}

static bool BolunIsValidMemoryMap(const BolunMemoryRegion *regions, size_t count)
{
    uint64_t previous_end = 0;

    for (size_t index = 0; index < count; ++index) {
        const BolunMemoryRegion *region = &regions[index];
        if (region->start >= region->end) {
            return false;
        }
        if (!BolunIsPageAligned(region->start) || !BolunIsPageAligned(region->end)) {
            return false;
        }
        if (region->start < previous_end) {
            return false;
        }
        previous_end = region->end;
    }

    return true;
}

BolunBootStatus BolunValidateBootInfo(const BolunBootInfo *boot_info)
{
    if (boot_info == NULL) {
        return BOLUN_BOOT_STATUS_BAD_ARGUMENT;
    }
    if (boot_info->magic != BOLUN_BOOT_MAGIC) {
        return BOLUN_BOOT_STATUS_BAD_MAGIC;
    }
    if (!BolunIsSupportedCpu(boot_info->cpu_architecture)) {
        return BOLUN_BOOT_STATUS_UNSUPPORTED_CPU;
    }
    if (!BolunIsSupportedSoc(boot_info->soc_family)) {
        return BOLUN_BOOT_STATUS_UNSUPPORTED_SOC;
    }
    if (boot_info->kernel_physical_start >= boot_info->kernel_physical_end) {
        return BOLUN_BOOT_STATUS_INVALID_KERNEL_RANGE;
    }
    if (boot_info->memory_region_count == 0U || boot_info->memory_regions == NULL) {
        return BOLUN_BOOT_STATUS_EMPTY_MEMORY_MAP;
    }
    if (boot_info->memory_region_count > BOLUN_MAX_BOOT_MEMORY_REGIONS) {
        return BOLUN_BOOT_STATUS_TOO_MANY_MEMORY_REGIONS;
    }
    if (!BolunIsValidMemoryMap(boot_info->memory_regions, boot_info->memory_region_count)) {
        return BOLUN_BOOT_STATUS_INVALID_MEMORY_MAP;
    }

    return BOLUN_BOOT_STATUS_OK;
}

const char *BolunBootStatusName(BolunBootStatus status)
{
    switch (status) {
    case BOLUN_BOOT_STATUS_OK:
        return "OK";
    case BOLUN_BOOT_STATUS_BAD_ARGUMENT:
        return "BAD_ARGUMENT";
    case BOLUN_BOOT_STATUS_BAD_MAGIC:
        return "BAD_MAGIC";
    case BOLUN_BOOT_STATUS_UNSUPPORTED_CPU:
        return "UNSUPPORTED_CPU";
    case BOLUN_BOOT_STATUS_UNSUPPORTED_SOC:
        return "UNSUPPORTED_SOC";
    case BOLUN_BOOT_STATUS_INVALID_KERNEL_RANGE:
        return "INVALID_KERNEL_RANGE";
    case BOLUN_BOOT_STATUS_EMPTY_MEMORY_MAP:
        return "EMPTY_MEMORY_MAP";
    case BOLUN_BOOT_STATUS_TOO_MANY_MEMORY_REGIONS:
        return "TOO_MANY_MEMORY_REGIONS";
    case BOLUN_BOOT_STATUS_INVALID_MEMORY_MAP:
        return "INVALID_MEMORY_MAP";
    default:
        return "UNKNOWN";
    }
}
