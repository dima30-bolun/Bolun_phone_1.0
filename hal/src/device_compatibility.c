#include "bolun/hal/device_compatibility.h"

static const BolunSocProfile kSupportedSocProfiles[] = {
    {BOLUN_SOC_QUALCOMM_SNAPDRAGON_S4, "Qualcomm Snapdragon S4", "Krait", BOLUN_DEVICE_QUIRK_SECURE_BOOT_CHAIN | BOLUN_DEVICE_QUIRK_LEGACY_SENSOR_HUB, 2U, 2U},
    {BOLUN_SOC_QUALCOMM_SNAPDRAGON_200, "Qualcomm Snapdragon 200", "Cortex-A5/A7", BOLUN_DEVICE_QUIRK_SECURE_BOOT_CHAIN | BOLUN_DEVICE_QUIRK_LEGACY_SENSOR_HUB, 2U, 4U},
    {BOLUN_SOC_QUALCOMM_SNAPDRAGON_400, "Qualcomm Snapdragon 400", "Cortex-A7/Krait", BOLUN_DEVICE_QUIRK_SECURE_BOOT_CHAIN | BOLUN_DEVICE_QUIRK_SHARED_DISPLAY_CLOCK, 2U, 4U},
    {BOLUN_SOC_QUALCOMM_SNAPDRAGON_800, "Qualcomm Snapdragon 800", "Krait 400", BOLUN_DEVICE_QUIRK_SECURE_BOOT_CHAIN | BOLUN_DEVICE_QUIRK_SHARED_DISPLAY_CLOCK, 4U, 4U},
    {BOLUN_SOC_QUALCOMM_SNAPDRAGON_808, "Qualcomm Snapdragon 808", "ARMv8 big.LITTLE", BOLUN_DEVICE_QUIRK_SECURE_BOOT_CHAIN | BOLUN_DEVICE_QUIRK_BIG_LITTLE_TOPOLOGY, 6U, 6U},
    {BOLUN_SOC_QUALCOMM_SNAPDRAGON_810, "Qualcomm Snapdragon 810", "ARMv8 big.LITTLE", BOLUN_DEVICE_QUIRK_SECURE_BOOT_CHAIN | BOLUN_DEVICE_QUIRK_BIG_LITTLE_TOPOLOGY, 8U, 8U},
};

const BolunSocProfile *BolunFindSocProfile(BolunSocFamily family)
{
    for (size_t index = 0; index < (sizeof(kSupportedSocProfiles) / sizeof(kSupportedSocProfiles[0])); ++index) {
        if (kSupportedSocProfiles[index].family == family) {
            return &kSupportedSocProfiles[index];
        }
    }

    return NULL;
}

bool BolunSocHasQuirk(const BolunSocProfile *profile, BolunDeviceQuirk quirk)
{
    if (profile == NULL) {
        return false;
    }

    return (profile->quirks & (unsigned int)quirk) != 0U;
}

size_t BolunEnumerateSupportedSocProfiles(const BolunSocProfile **profiles, size_t capacity)
{
    const size_t count = sizeof(kSupportedSocProfiles) / sizeof(kSupportedSocProfiles[0]);
    const size_t copy_count = capacity < count ? capacity : count;

    if (profiles != NULL) {
        for (size_t index = 0; index < copy_count; ++index) {
            profiles[index] = &kSupportedSocProfiles[index];
        }
    }

    return count;
}
