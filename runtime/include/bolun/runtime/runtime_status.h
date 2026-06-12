#ifndef BOLUN_RUNTIME_RUNTIME_STATUS_H
#define BOLUN_RUNTIME_RUNTIME_STATUS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum BolunRuntimeStatus {
    BOLUN_RUNTIME_OK = 0,
    BOLUN_RUNTIME_PENDING = 1,
    BOLUN_RUNTIME_FAILED = 2
} BolunRuntimeStatus;

#ifdef __cplusplus
}
#endif

#endif
