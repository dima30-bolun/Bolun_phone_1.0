#ifndef BOLUN_HAL_DEVICE_COMPATIBILITY_H
#define BOLUN_HAL_DEVICE_COMPATIBILITY_H

#include <stdbool.h>
#include <stddef.h>

#include "bolun/boot/boot_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum BolunDeviceQuirk {
    BOLUN_DEVICE_QUIRK_NONE = 0,
    BOLUN_DEVICE_QUIRK_SECURE_BOOT_CHAIN = 1U << 0U,
    BOLUN_DEVICE_QUIRK_SHARED_DISPLAY_CLOCK = 1U << 1U,
    BOLUN_DEVICE_QUIRK_LEGACY_SENSOR_HUB = 1U << 2U,
    BOLUN_DEVICE_QUIRK_BIG_LITTLE_TOPOLOGY = 1U << 3U
} BolunDeviceQuirk;

typedef struct BolunSocProfile {
    BolunSocFamily family;
    const char *marketing_name;
    const char *cpu_generation;
    unsigned int quirks;
    unsigned int expected_core_count_min;
    unsigned int expected_core_count_max;
} BolunSocProfile;

const BolunSocProfile *BolunFindSocProfile(BolunSocFamily family);
bool BolunSocHasQuirk(const BolunSocProfile *profile, BolunDeviceQuirk quirk);
size_t BolunEnumerateSupportedSocProfiles(const BolunSocProfile **profiles, size_t capacity);

#ifdef __cplusplus
}
#endif

#endif
