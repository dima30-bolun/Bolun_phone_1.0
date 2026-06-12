#ifndef BOLUN_SDK_NATIVE_API_H
#define BOLUN_SDK_NATIVE_API_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t BolunAppHandle;

typedef enum BolunAppCapability {
    BOLUN_APP_CAPABILITY_NETWORK = 1U << 0U,
    BOLUN_APP_CAPABILITY_STORAGE = 1U << 1U,
    BOLUN_APP_CAPABILITY_NOTIFICATIONS = 1U << 2U
} BolunAppCapability;

#ifdef __cplusplus
}
#endif

#endif
