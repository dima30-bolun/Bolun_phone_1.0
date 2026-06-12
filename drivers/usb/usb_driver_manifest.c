#include "bolun/drivers/driver_abstraction.h"

const BolunDriverDescriptor kBolunUsbDriverManifest = {
    .name = "bolun-usb-class",
    .driver_class = BOLUN_DRIVER_CLASS_USB,
    .preferred_soc_family = BOLUN_SOC_QUALCOMM_SNAPDRAGON_400,
    .compatible_soc_mask = BOLUN_SOC_MASK_ALL_QUALCOMM_LUMIA,
    .abi_version = 1U,
};
