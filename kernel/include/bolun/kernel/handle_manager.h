#ifndef BOLUN_KERNEL_HANDLE_MANAGER_H
#define BOLUN_KERNEL_HANDLE_MANAGER_H

#include <stddef.h>
#include <stdint.h>

#include "bolun/kernel/security.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BOLUN_HANDLE_TABLE_CAPACITY 128U

typedef uint32_t BolunHandle;

typedef struct BolunHandleTableEntry {
    BolunHandle handle;
    BolunCapability capability;
    uint32_t owner_process_id;
} BolunHandleTableEntry;

typedef struct BolunHandleTable {
    BolunHandleTableEntry entries[BOLUN_HANDLE_TABLE_CAPACITY];
    size_t length;
    BolunHandle next_handle;
} BolunHandleTable;

typedef enum BolunHandleStatus {
    BOLUN_HANDLE_OK = 0,
    BOLUN_HANDLE_BAD_ARGUMENT = 1,
    BOLUN_HANDLE_TABLE_FULL = 2,
    BOLUN_HANDLE_NOT_FOUND = 3,
    BOLUN_HANDLE_ACCESS_DENIED = 4
} BolunHandleStatus;

void BolunHandleTableInitialize(BolunHandleTable *table);
BolunHandleStatus BolunHandleOpen(BolunHandleTable *table,
                                  uint32_t owner_process_id,
                                  BolunCapability capability,
                                  BolunHandle *handle);
BolunHandleStatus BolunHandleResolve(const BolunHandleTable *table,
                                     uint32_t owner_process_id,
                                     BolunHandle handle,
                                     unsigned int requested_rights,
                                     BolunCapability *capability);

#ifdef __cplusplus
}
#endif

#endif
