#ifndef BOLUN_DRIVERS_DRIVER_ABSTRACTION_H
#define BOLUN_DRIVERS_DRIVER_ABSTRACTION_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "bolun/hal/device_compatibility.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum BolunDriverClass {
    BOLUN_DRIVER_CLASS_DISPLAY = 1,
    BOLUN_DRIVER_CLASS_TOUCH = 2,
    BOLUN_DRIVER_CLASS_SENSOR = 3,
    BOLUN_DRIVER_CLASS_STORAGE = 4,
    BOLUN_DRIVER_CLASS_NETWORK = 5,
    BOLUN_DRIVER_CLASS_POWER = 6,
    BOLUN_DRIVER_CLASS_AUDIO = 7,
    BOLUN_DRIVER_CLASS_CAMERA = 8,
    BOLUN_DRIVER_CLASS_BLUETOOTH = 9,
    BOLUN_DRIVER_CLASS_WIFI = 10,
    BOLUN_DRIVER_CLASS_GPS = 11,
    BOLUN_DRIVER_CLASS_NFC = 12,
    BOLUN_DRIVER_CLASS_USB = 13,
    BOLUN_DRIVER_CLASS_CELLULAR = 14
} BolunDriverClass;

#define BOLUN_SOC_MASK_SNAPDRAGON_S4 (1UL << 0U)
#define BOLUN_SOC_MASK_SNAPDRAGON_200 (1UL << 1U)
#define BOLUN_SOC_MASK_SNAPDRAGON_400 (1UL << 2U)
#define BOLUN_SOC_MASK_SNAPDRAGON_800 (1UL << 3U)
#define BOLUN_SOC_MASK_SNAPDRAGON_808 (1UL << 4U)
#define BOLUN_SOC_MASK_SNAPDRAGON_810 (1UL << 5U)
#define BOLUN_SOC_MASK_ALL_QUALCOMM_LUMIA \
    (BOLUN_SOC_MASK_SNAPDRAGON_S4 | BOLUN_SOC_MASK_SNAPDRAGON_200 | \
     BOLUN_SOC_MASK_SNAPDRAGON_400 | BOLUN_SOC_MASK_SNAPDRAGON_800 | \
     BOLUN_SOC_MASK_SNAPDRAGON_808 | BOLUN_SOC_MASK_SNAPDRAGON_810)

typedef struct BolunDriverDescriptor {
    const char *name;
    BolunDriverClass driver_class;
    BolunSocFamily preferred_soc_family;
    unsigned long compatible_soc_mask;
    uint32_t abi_version;
} BolunDriverDescriptor;

typedef enum BolunDriverMatchResult {
    BOLUN_DRIVER_MATCH_NO = 0,
    BOLUN_DRIVER_MATCH_GENERIC = 1,
    BOLUN_DRIVER_MATCH_EXACT = 2
} BolunDriverMatchResult;

unsigned long BolunSocFamilyToMask(BolunSocFamily family);
BolunDriverMatchResult BolunMatchDriverToSoc(const BolunDriverDescriptor *driver,
                                             const BolunSocProfile *profile);
bool BolunIsDisplayCriticalDriver(BolunDriverClass driver_class);

#ifdef __cplusplus
}
#endif

#endif
