#ifndef BOLUN_UPDATEOS_UPDATE_PLAN_H
#define BOLUN_UPDATEOS_UPDATE_PLAN_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct BolunUpdatePackageHeader {
    uint32_t version_major;
    uint32_t version_minor;
    uint64_t payload_size_bytes;
    uint32_t rollback_index;
} BolunUpdatePackageHeader;

#ifdef __cplusplus
}
#endif

#endif
