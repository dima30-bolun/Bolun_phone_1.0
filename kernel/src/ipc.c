#include "bolun/kernel/ipc.h"

BolunIpcStatus BolunIpcValidateMessage(const BolunIpcMessage *message)
{
    if (message == NULL || message->sender_process_id == 0U) {
        return BOLUN_IPC_BAD_ARGUMENT;
    }
    if (message->target_port_id == 0U) {
        return BOLUN_IPC_INVALID_TARGET;
    }
    if (message->handle_count > BOLUN_IPC_MAX_HANDLES) {
        return BOLUN_IPC_TOO_MANY_HANDLES;
    }

    return BOLUN_IPC_OK;
}
