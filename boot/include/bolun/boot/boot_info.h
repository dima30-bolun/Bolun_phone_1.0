#ifndef BOLUN_BOOT_BOOT_INFO_H
#define BOLUN_BOOT_BOOT_INFO_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BOLUN_BOOT_MAGIC UINT64_C(0x424F4C554E313042)
#define BOLUN_MAX_BOOT_MEMORY_REGIONS 128U
#define BOLUN_PAGE_SIZE UINT64_C(4096)

typedef enum BolunCpuArchitecture {
    BOLUN_CPU_ARCH_AARCH32 = 1,
    BOLUN_CPU_ARCH_AARCH64 = 2,
    BOLUN_CPU_ARCH_X86 = 3,
    BOLUN_CPU_ARCH_X86_64 = 4
} BolunCpuArchitecture;

typedef enum BolunMemoryKind {
    BOLUN_MEMORY_USABLE = 1,
    BOLUN_MEMORY_RESERVED = 2,
    BOLUN_MEMORY_DEVICE = 3,
    BOLUN_MEMORY_PERSISTENT = 4,
    BOLUN_MEMORY_BOOTLOADER = 5,
    BOLUN_MEMORY_FRAMEBUFFER = 6
} BolunMemoryKind;

typedef struct BolunMemoryRegion {
    uint64_t start;
    uint64_t end;
    BolunMemoryKind kind;
} BolunMemoryRegion;

typedef enum BolunSocFamily {
    BOLUN_SOC_UNKNOWN = 0,
    BOLUN_SOC_QUALCOMM_SNAPDRAGON_S4 = 100,
    BOLUN_SOC_QUALCOMM_SNAPDRAGON_200 = 200,
    BOLUN_SOC_QUALCOMM_SNAPDRAGON_400 = 400,
    BOLUN_SOC_QUALCOMM_SNAPDRAGON_800 = 800,
    BOLUN_SOC_QUALCOMM_SNAPDRAGON_808 = 808,
    BOLUN_SOC_QUALCOMM_SNAPDRAGON_810 = 810
} BolunSocFamily;

typedef struct BolunBootInfo {
    uint64_t magic;
    BolunCpuArchitecture cpu_architecture;
    BolunSocFamily soc_family;
    uint64_t kernel_physical_start;
    uint64_t kernel_physical_end;
    const BolunMemoryRegion *memory_regions;
    size_t memory_region_count;
    uint64_t device_tree_physical;
    uint64_t acpi_rsdp_physical;
    uint64_t framebuffer_physical;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint32_t framebuffer_stride;
} BolunBootInfo;

typedef enum BolunBootStatus {
    BOLUN_BOOT_STATUS_OK = 0,
    BOLUN_BOOT_STATUS_BAD_ARGUMENT = 1,
    BOLUN_BOOT_STATUS_BAD_MAGIC = 2,
    BOLUN_BOOT_STATUS_UNSUPPORTED_CPU = 3,
    BOLUN_BOOT_STATUS_UNSUPPORTED_SOC = 4,
    BOLUN_BOOT_STATUS_INVALID_KERNEL_RANGE = 5,
    BOLUN_BOOT_STATUS_EMPTY_MEMORY_MAP = 6,
    BOLUN_BOOT_STATUS_TOO_MANY_MEMORY_REGIONS = 7,
    BOLUN_BOOT_STATUS_INVALID_MEMORY_MAP = 8
} BolunBootStatus;

BolunBootStatus BolunValidateBootInfo(const BolunBootInfo *boot_info);
const char *BolunBootStatusName(BolunBootStatus status);

#ifdef __cplusplus
}
#endif

#endif
