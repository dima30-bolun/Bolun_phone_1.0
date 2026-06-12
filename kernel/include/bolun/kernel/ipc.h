#ifndef BOLUN_KERNEL_IPC_H
#define BOLUN_KERNEL_IPC_H

#include <stddef.h>
#include <stdint.h>

#include "bolun/kernel/handle_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BOLUN_IPC_PAYLOAD_WORDS 8U
#define BOLUN_IPC_MAX_HANDLES 4U

typedef struct BolunIpcMessage {
    uint32_t sender_process_id;
    uint32_t target_port_id;
    uint32_t payload_words[BOLUN_IPC_PAYLOAD_WORDS];
    BolunHandle handles[BOLUN_IPC_MAX_HANDLES];
    size_t handle_count;
} BolunIpcMessage;

typedef enum BolunIpcStatus {
    BOLUN_IPC_OK = 0,
    BOLUN_IPC_BAD_ARGUMENT = 1,
    BOLUN_IPC_TOO_MANY_HANDLES = 2,
    BOLUN_IPC_INVALID_TARGET = 3
} BolunIpcStatus;

BolunIpcStatus BolunIpcValidateMessage(const BolunIpcMessage *message);

#ifdef __cplusplus
}
#endif

#endif
