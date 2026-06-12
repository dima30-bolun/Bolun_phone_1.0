#include "bolun/drivers/driver_abstraction.h"

const BolunDriverDescriptor kBolunLumiaTouchDriver = {
    .name = "lumia-touch-class",
    .driver_class = BOLUN_DRIVER_CLASS_TOUCH,
    .preferred_soc_family = BOLUN_SOC_QUALCOMM_SNAPDRAGON_400,
    .compatible_soc_mask = BOLUN_SOC_MASK_ALL_QUALCOMM_LUMIA,
    .abi_version = 1U,
};
