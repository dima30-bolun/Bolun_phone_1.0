#include "bolun/kernel/handle_manager.h"

void BolunHandleTableInitialize(BolunHandleTable *table)
{
    if (table == NULL) {
        return;
    }

    table->length = 0;
    table->next_handle = 1U;
    for (size_t index = 0; index < BOLUN_HANDLE_TABLE_CAPACITY; ++index) {
        table->entries[index].handle = 0U;
        table->entries[index].owner_process_id = 0U;
        table->entries[index].capability = BolunCapabilityCreate(0U, BOLUN_OBJECT_PROCESS, BOLUN_ACCESS_NONE);
    }
}

BolunHandleStatus BolunHandleOpen(BolunHandleTable *table,
                                  uint32_t owner_process_id,
                                  BolunCapability capability,
                                  BolunHandle *handle)
{
    if (table == NULL || handle == NULL || owner_process_id == 0U || capability.object_id == 0U) {
        return BOLUN_HANDLE_BAD_ARGUMENT;
    }
    if (table->length >= BOLUN_HANDLE_TABLE_CAPACITY) {
        return BOLUN_HANDLE_TABLE_FULL;
    }

    BolunHandle assigned = table->next_handle;
    ++table->next_handle;
    table->entries[table->length].handle = assigned;
    table->entries[table->length].owner_process_id = owner_process_id;
    table->entries[table->length].capability = capability;
    ++table->length;
    *handle = assigned;
    return BOLUN_HANDLE_OK;
}

BolunHandleStatus BolunHandleResolve(const BolunHandleTable *table,
                                     uint32_t owner_process_id,
                                     BolunHandle handle,
                                     unsigned int requested_rights,
                                     BolunCapability *capability)
{
    if (table == NULL || capability == NULL || owner_process_id == 0U || handle == 0U) {
        return BOLUN_HANDLE_BAD_ARGUMENT;
    }

    for (size_t index = 0; index < table->length; ++index) {
        const BolunHandleTableEntry *entry = &table->entries[index];
        if (entry->handle == handle && entry->owner_process_id == owner_process_id) {
            if (!BolunCapabilityAllows(entry->capability, requested_rights)) {
                return BOLUN_HANDLE_ACCESS_DENIED;
            }
            *capability = entry->capability;
            return BOLUN_HANDLE_OK;
        }
    }

    return BOLUN_HANDLE_NOT_FOUND;
}
