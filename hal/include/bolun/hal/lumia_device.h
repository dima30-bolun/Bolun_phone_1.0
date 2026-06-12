#ifndef BOLUN_HAL_LUMIA_DEVICE_H
#define BOLUN_HAL_LUMIA_DEVICE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "bolun/boot/boot_info.h"
#include "bolun/hal/device_compatibility.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum BolunLumiaDeviceId {
    BOLUN_LUMIA_520 = 520,
    BOLUN_LUMIA_525 = 525,
    BOLUN_LUMIA_530 = 530,
    BOLUN_LUMIA_532 = 532,
    BOLUN_LUMIA_535 = 535,
    BOLUN_LUMIA_540 = 540,
    BOLUN_LUMIA_550 = 550,
    BOLUN_LUMIA_620 = 620,
    BOLUN_LUMIA_625 = 625,
    BOLUN_LUMIA_630 = 630,
    BOLUN_LUMIA_635 = 635,
    BOLUN_LUMIA_640 = 640,
    BOLUN_LUMIA_640_XL = 641,
    BOLUN_LUMIA_650 = 650,
    BOLUN_LUMIA_720 = 720,
    BOLUN_LUMIA_730 = 730,
    BOLUN_LUMIA_735 = 735,
    BOLUN_LUMIA_820 = 820,
    BOLUN_LUMIA_920 = 920,
    BOLUN_LUMIA_925 = 925,
    BOLUN_LUMIA_930 = 930,
    BOLUN_LUMIA_1020 = 1020,
    BOLUN_LUMIA_1320 = 1320,
    BOLUN_LUMIA_1520 = 1520,
    BOLUN_LUMIA_950 = 950,
    BOLUN_LUMIA_950_XL = 951
} BolunLumiaDeviceId;

typedef enum BolunLumiaFeature {
    BOLUN_LUMIA_FEATURE_NONE = 0,
    BOLUN_LUMIA_FEATURE_TOUCH = 1U << 0U,
    BOLUN_LUMIA_FEATURE_CAMERA = 1U << 1U,
    BOLUN_LUMIA_FEATURE_FRONT_CAMERA = 1U << 2U,
    BOLUN_LUMIA_FEATURE_NFC = 1U << 3U,
    BOLUN_LUMIA_FEATURE_LTE = 1U << 4U,
    BOLUN_LUMIA_FEATURE_SENSOR_CORE = 1U << 5U,
    BOLUN_LUMIA_FEATURE_GLANCE_DISPLAY = 1U << 6U,
    BOLUN_LUMIA_FEATURE_CONTINUUM_CLASS = 1U << 7U
} BolunLumiaFeature;

typedef struct BolunLumiaBoardProfile {
    BolunLumiaDeviceId id;
    const char *model_name;
    BolunSocFamily soc_family;
    BolunCpuArchitecture cpu_architecture;
    uint32_t display_width;
    uint32_t display_height;
    uint32_t ram_megabytes;
    unsigned int features;
} BolunLumiaBoardProfile;

typedef enum BolunLumiaCompatibilityStatus {
    BOLUN_LUMIA_COMPATIBLE = 0,
    BOLUN_LUMIA_UNSUPPORTED_DEVICE = 1,
    BOLUN_LUMIA_CPU_SOC_MISMATCH = 2,
    BOLUN_LUMIA_DISPLAY_MISMATCH = 3,
    BOLUN_LUMIA_INSUFFICIENT_RAM = 4
} BolunLumiaCompatibilityStatus;

const BolunLumiaBoardProfile *BolunFindLumiaBoardProfile(BolunLumiaDeviceId id);
size_t BolunEnumerateLumiaBoardProfiles(const BolunLumiaBoardProfile **profiles, size_t capacity);
bool BolunLumiaHasFeature(const BolunLumiaBoardProfile *profile, BolunLumiaFeature feature);
BolunLumiaCompatibilityStatus BolunCheckLumiaCompatibility(const BolunLumiaBoardProfile *profile,
                                                           const BolunBootInfo *boot_info);

#ifdef __cplusplus
}
#endif

#endif
