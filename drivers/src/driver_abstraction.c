#include "bolun/drivers/driver_abstraction.h"

unsigned long BolunSocFamilyToMask(BolunSocFamily family)
{
    switch (family) {
    case BOLUN_SOC_QUALCOMM_SNAPDRAGON_S4:
        return BOLUN_SOC_MASK_SNAPDRAGON_S4;
    case BOLUN_SOC_QUALCOMM_SNAPDRAGON_200:
        return BOLUN_SOC_MASK_SNAPDRAGON_200;
    case BOLUN_SOC_QUALCOMM_SNAPDRAGON_400:
        return BOLUN_SOC_MASK_SNAPDRAGON_400;
    case BOLUN_SOC_QUALCOMM_SNAPDRAGON_800:
        return BOLUN_SOC_MASK_SNAPDRAGON_800;
    case BOLUN_SOC_QUALCOMM_SNAPDRAGON_808:
        return BOLUN_SOC_MASK_SNAPDRAGON_808;
    case BOLUN_SOC_QUALCOMM_SNAPDRAGON_810:
        return BOLUN_SOC_MASK_SNAPDRAGON_810;
    case BOLUN_SOC_UNKNOWN:
    default:
        return 0UL;
    }
}

BolunDriverMatchResult BolunMatchDriverToSoc(const BolunDriverDescriptor *driver,
                                             const BolunSocProfile *profile)
{
    if (driver == NULL || profile == NULL) {
        return BOLUN_DRIVER_MATCH_NO;
    }

    const unsigned long profile_mask = BolunSocFamilyToMask(profile->family);
    if ((driver->compatible_soc_mask & profile_mask) == 0UL) {
        return BOLUN_DRIVER_MATCH_NO;
    }

    if (driver->preferred_soc_family == profile->family) {
        return BOLUN_DRIVER_MATCH_EXACT;
    }

    return BOLUN_DRIVER_MATCH_GENERIC;
}

bool BolunIsDisplayCriticalDriver(BolunDriverClass driver_class)
{
    return driver_class == BOLUN_DRIVER_CLASS_DISPLAY || driver_class == BOLUN_DRIVER_CLASS_TOUCH;
}
