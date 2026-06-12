#ifndef BOLUN_KERNEL_SECURITY_H
#define BOLUN_KERNEL_SECURITY_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum BolunKernelObjectType {
    BOLUN_OBJECT_THREAD = 1,
    BOLUN_OBJECT_PROCESS = 2,
    BOLUN_OBJECT_ADDRESS_SPACE = 3,
    BOLUN_OBJECT_IPC_PORT = 4,
    BOLUN_OBJECT_DRIVER = 5,
    BOLUN_OBJECT_DEVICE = 6
} BolunKernelObjectType;

typedef enum BolunAccessRights {
    BOLUN_ACCESS_NONE = 0,
    BOLUN_ACCESS_READ = 1U << 0U,
    BOLUN_ACCESS_WRITE = 1U << 1U,
    BOLUN_ACCESS_EXECUTE = 1U << 2U,
    BOLUN_ACCESS_MAP = 1U << 3U,
    BOLUN_ACCESS_SIGNAL = 1U << 4U,
    BOLUN_ACCESS_TRANSFER = 1U << 5U,
    BOLUN_ACCESS_ADMIN = 1U << 6U
} BolunAccessRights;

typedef struct BolunCapability {
    uint32_t object_id;
    BolunKernelObjectType object_type;
    unsigned int rights;
} BolunCapability;

BolunCapability BolunCapabilityCreate(uint32_t object_id, BolunKernelObjectType object_type, unsigned int rights);
bool BolunCapabilityAllows(BolunCapability capability, unsigned int requested_rights);

#ifdef __cplusplus
}
#endif

#endif
