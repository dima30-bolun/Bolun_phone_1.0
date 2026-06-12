#include "bolun/boot/boot_config.h"

#include <string.h>

static bool BolunIsValidEntry(const BolunBootEntry *entry)
{
    return entry != NULL && entry->mode >= BOLUN_BOOT_MODE_MAINOS &&
           entry->mode <= BOLUN_BOOT_MODE_DIAGNOSTICS && entry->image_size_bytes > 0U &&
           entry->name[0] != '\0';
}

void BolunBootConfigurationInitialize(BolunBootConfiguration *configuration)
{
    if (configuration == NULL) {
        return;
    }

    memset(configuration, 0, sizeof(*configuration));
    configuration->default_mode = BOLUN_BOOT_MODE_MAINOS;
    configuration->boot_timeout_milliseconds = 3000U;
}

BolunBootManagerStatus BolunBootConfigurationAddEntry(BolunBootConfiguration *configuration,
                                                      BolunBootEntry entry)
{
    if (configuration == NULL) {
        return BOLUN_BOOT_MANAGER_BAD_ARGUMENT;
    }
    if (configuration->entry_count >= BOLUN_BOOT_CONFIG_MAX_ENTRIES) {
        return BOLUN_BOOT_MANAGER_INVALID_ENTRY;
    }
    if (!BolunIsValidEntry(&entry)) {
        return BOLUN_BOOT_MANAGER_INVALID_ENTRY;
    }

    configuration->entries[configuration->entry_count] = entry;
    ++configuration->entry_count;
    return BOLUN_BOOT_MANAGER_OK;
}

BolunBootManagerStatus BolunBootSelectEntry(const BolunBootConfiguration *configuration,
                                            BolunBootMode requested_mode,
                                            const BolunBootEntry **selected_entry)
{
    BolunBootMode target_mode;

    if (configuration == NULL || selected_entry == NULL) {
        return BOLUN_BOOT_MANAGER_BAD_ARGUMENT;
    }

    *selected_entry = NULL;
    target_mode = requested_mode == 0 ? configuration->default_mode : requested_mode;

    for (size_t index = 0; index < configuration->entry_count; ++index) {
        const BolunBootEntry *entry = &configuration->entries[index];
        if (entry->enabled && entry->mode == target_mode) {
            *selected_entry = entry;
            return BOLUN_BOOT_MANAGER_OK;
        }
    }

    for (size_t index = 0; index < configuration->entry_count; ++index) {
        const BolunBootEntry *entry = &configuration->entries[index];
        if (entry->enabled && entry->mode == configuration->default_mode) {
            *selected_entry = entry;
            return BOLUN_BOOT_MANAGER_OK;
        }
    }

    return BOLUN_BOOT_MANAGER_NO_ENABLED_ENTRY;
}
