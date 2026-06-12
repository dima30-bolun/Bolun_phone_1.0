#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "bolun/boot/boot_config.h"
#include "bolun/boot/boot_info.h"
#include "bolun/drivers/driver_abstraction.h"
#include "bolun/hal/device_compatibility.h"
#include "bolun/hal/lumia_device.h"
#include "bolun/kernel/handle_manager.h"
#include "bolun/kernel/ipc.h"
#include "bolun/kernel/memory.h"
#include "bolun/kernel/scheduler.h"
#include "bolun/kernel/security.h"


static void TestBootManagerSelectsRecoveryFallback(void)
{
    BolunBootConfiguration configuration;
    const BolunBootEntry *selected = NULL;

    BolunBootConfigurationInitialize(&configuration);
    assert(BolunBootConfigurationAddEntry(&configuration, (BolunBootEntry){
        .mode = BOLUN_BOOT_MODE_MAINOS,
        .name = "MainOS",
        .image_physical_start = 0x00200000ULL,
        .image_size_bytes = 0x01000000ULL,
        .enabled = true,
    }) == BOLUN_BOOT_MANAGER_OK);
    assert(BolunBootConfigurationAddEntry(&configuration, (BolunBootEntry){
        .mode = BOLUN_BOOT_MODE_RECOVERY,
        .name = "Recovery",
        .image_physical_start = 0x02000000ULL,
        .image_size_bytes = 0x00400000ULL,
        .enabled = true,
    }) == BOLUN_BOOT_MANAGER_OK);

    assert(BolunBootSelectEntry(&configuration, BOLUN_BOOT_MODE_RECOVERY, &selected) == BOLUN_BOOT_MANAGER_OK);
    assert(selected != NULL);
    assert(selected->mode == BOLUN_BOOT_MODE_RECOVERY);
}

static void TestBootInfoAcceptsRealLumiaClassSoc(void)
{
    const BolunMemoryRegion regions[] = {
        {0x00000000ULL, 0x00001000ULL, BOLUN_MEMORY_RESERVED},
        {0x00001000ULL, 0x00101000ULL, BOLUN_MEMORY_USABLE},
        {0x10000000ULL, 0x10001000ULL, BOLUN_MEMORY_DEVICE},
    };
    const BolunBootInfo boot_info = {
        .magic = BOLUN_BOOT_MAGIC,
        .cpu_architecture = BOLUN_CPU_ARCH_AARCH32,
        .soc_family = BOLUN_SOC_QUALCOMM_SNAPDRAGON_400,
        .kernel_physical_start = 0x00002000ULL,
        .kernel_physical_end = 0x00008000ULL,
        .memory_regions = regions,
        .memory_region_count = sizeof(regions) / sizeof(regions[0]),
        .device_tree_physical = 0x00080000ULL,
        .acpi_rsdp_physical = 0ULL,
        .framebuffer_physical = 0x10000000ULL,
        .framebuffer_width = 720U,
        .framebuffer_height = 1280U,
        .framebuffer_stride = 720U * 4U,
    };

    assert(BolunValidateBootInfo(&boot_info) == BOLUN_BOOT_STATUS_OK);
    assert(BolunCountUsablePages(regions, sizeof(regions) / sizeof(regions[0])) == 256ULL);
}

static void TestDeviceCompatibilityLayerListsRequiredFamilies(void)
{
    const BolunSocProfile *profiles[8] = {0};
    const size_t total = BolunEnumerateSupportedSocProfiles(profiles, 8U);

    assert(total == 6U);
    assert(BolunFindSocProfile(BOLUN_SOC_QUALCOMM_SNAPDRAGON_S4) != NULL);
    assert(BolunFindSocProfile(BOLUN_SOC_QUALCOMM_SNAPDRAGON_200) != NULL);
    assert(BolunFindSocProfile(BOLUN_SOC_QUALCOMM_SNAPDRAGON_400) != NULL);
    assert(BolunFindSocProfile(BOLUN_SOC_QUALCOMM_SNAPDRAGON_800) != NULL);
    assert(BolunFindSocProfile(BOLUN_SOC_QUALCOMM_SNAPDRAGON_808) != NULL);
    assert(BolunFindSocProfile(BOLUN_SOC_QUALCOMM_SNAPDRAGON_810) != NULL);
    assert(BolunSocHasQuirk(BolunFindSocProfile(BOLUN_SOC_QUALCOMM_SNAPDRAGON_810),
                            BOLUN_DEVICE_QUIRK_BIG_LITTLE_TOPOLOGY));
}


static void TestLumiaCompatibilityLayerMatchesBootInfo(void)
{
    const BolunMemoryRegion regions[] = {
        {0x00000000ULL, 0x00001000ULL, BOLUN_MEMORY_RESERVED},
        {0x00001000ULL, 0x00201000ULL, BOLUN_MEMORY_USABLE},
    };
    const BolunLumiaBoardProfile *profile = BolunFindLumiaBoardProfile(BOLUN_LUMIA_950_XL);
    const BolunBootInfo boot_info = {
        .magic = BOLUN_BOOT_MAGIC,
        .cpu_architecture = BOLUN_CPU_ARCH_AARCH64,
        .soc_family = BOLUN_SOC_QUALCOMM_SNAPDRAGON_810,
        .kernel_physical_start = 0x00002000ULL,
        .kernel_physical_end = 0x00008000ULL,
        .memory_regions = regions,
        .memory_region_count = sizeof(regions) / sizeof(regions[0]),
        .device_tree_physical = 0x00080000ULL,
        .acpi_rsdp_physical = 0ULL,
        .framebuffer_physical = 0x10000000ULL,
        .framebuffer_width = 1440U,
        .framebuffer_height = 2560U,
        .framebuffer_stride = 1440U * 4U,
    };

    assert(profile != NULL);
    assert(BolunEnumerateLumiaBoardProfiles(NULL, 0U) == 26U);
    assert(BolunLumiaHasFeature(profile, BOLUN_LUMIA_FEATURE_CONTINUUM_CLASS));
    assert(BolunCheckLumiaCompatibility(profile, &boot_info) == BOLUN_LUMIA_COMPATIBLE);
}

static void TestKernelSecurityHandlesAndIpc(void)
{
    BolunHandleTable table;
    BolunHandle handle = 0U;
    BolunCapability resolved = BolunCapabilityCreate(0U, BOLUN_OBJECT_PROCESS, BOLUN_ACCESS_NONE);
    const BolunCapability port = BolunCapabilityCreate(
        42U,
        BOLUN_OBJECT_IPC_PORT,
        BOLUN_ACCESS_READ | BOLUN_ACCESS_WRITE | BOLUN_ACCESS_TRANSFER);
    const BolunIpcMessage message = {
        .sender_process_id = 7U,
        .target_port_id = 42U,
        .payload_words = {1U, 2U, 3U, 4U, 0U, 0U, 0U, 0U},
        .handles = {0U, 0U, 0U, 0U},
        .handle_count = 0U,
    };

    BolunHandleTableInitialize(&table);
    assert(BolunHandleOpen(&table, 7U, port, &handle) == BOLUN_HANDLE_OK);
    assert(BolunHandleResolve(&table, 7U, handle, BOLUN_ACCESS_WRITE, &resolved) == BOLUN_HANDLE_OK);
    assert(resolved.object_id == 42U);
    assert(BolunHandleResolve(&table, 7U, handle, BOLUN_ACCESS_ADMIN, &resolved) == BOLUN_HANDLE_ACCESS_DENIED);
    assert(BolunIpcValidateMessage(&message) == BOLUN_IPC_OK);
}

static void TestDriverAbstractionLayerMatchesDisplayDriver(void)
{
    const BolunSocProfile *profile = BolunFindSocProfile(BOLUN_SOC_QUALCOMM_SNAPDRAGON_800);
    const BolunDriverDescriptor display_driver = {
        .name = "msm-display-class",
        .driver_class = BOLUN_DRIVER_CLASS_DISPLAY,
        .preferred_soc_family = BOLUN_SOC_QUALCOMM_SNAPDRAGON_400,
        .compatible_soc_mask = BOLUN_SOC_MASK_SNAPDRAGON_400 | BOLUN_SOC_MASK_SNAPDRAGON_800,
        .abi_version = 1U,
    };

    assert(BolunMatchDriverToSoc(&display_driver, profile) == BOLUN_DRIVER_MATCH_GENERIC);
    assert(BolunIsDisplayCriticalDriver(display_driver.driver_class));
}

static void TestEarlySchedulerPriorityPolicy(void)
{
    BolunEarlyRunQueue queue;
    BolunRunnableThread selected = {{0U}, 0U, 0U};

    BolunEarlyRunQueueInitialize(&queue);
    assert(BolunEarlyRunQueuePush(&queue, (BolunRunnableThread){{1U}, 32U, 20U}) == BOLUN_SCHEDULER_STATUS_OK);
    assert(BolunEarlyRunQueuePush(&queue, (BolunRunnableThread){{2U}, 224U, 30U}) == BOLUN_SCHEDULER_STATUS_OK);
    assert(BolunEarlyRunQueuePush(&queue, (BolunRunnableThread){{3U}, 128U, 10U}) == BOLUN_SCHEDULER_STATUS_OK);

    assert(BolunEarlyRunQueuePopNext(&queue, &selected) == BOLUN_SCHEDULER_STATUS_OK);
    assert(selected.thread_id.value == 2U);
    assert(BolunEarlyRunQueuePopNext(&queue, &selected) == BOLUN_SCHEDULER_STATUS_OK);
    assert(selected.thread_id.value == 3U);
    assert(BolunEarlyRunQueuePopNext(&queue, &selected) == BOLUN_SCHEDULER_STATUS_OK);
    assert(selected.thread_id.value == 1U);
}

int main(void)
{
    TestBootManagerSelectsRecoveryFallback();
    TestBootInfoAcceptsRealLumiaClassSoc();
    TestDeviceCompatibilityLayerListsRequiredFamilies();
    TestLumiaCompatibilityLayerMatchesBootInfo();
    TestDriverAbstractionLayerMatchesDisplayDriver();
    TestKernelSecurityHandlesAndIpc();
    TestEarlySchedulerPriorityPolicy();
    return 0;
}
