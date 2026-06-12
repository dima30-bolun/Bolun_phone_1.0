#ifndef BOLUN_BOOT_BOOT_CONFIG_H
#define BOLUN_BOOT_BOOT_CONFIG_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BOLUN_BOOT_CONFIG_MAX_ENTRIES 8U
#define BOLUN_BOOT_NAME_MAX_LENGTH 32U

typedef enum BolunBootMode {
    BOLUN_BOOT_MODE_MAINOS = 1,
    BOLUN_BOOT_MODE_RECOVERY = 2,
    BOLUN_BOOT_MODE_UPDATEOS = 3,
    BOLUN_BOOT_MODE_DIAGNOSTICS = 4
} BolunBootMode;

typedef struct BolunBootEntry {
    BolunBootMode mode;
    char name[BOLUN_BOOT_NAME_MAX_LENGTH];
    uint64_t image_physical_start;
    uint64_t image_size_bytes;
    bool enabled;
} BolunBootEntry;

typedef struct BolunBootConfiguration {
    BolunBootEntry entries[BOLUN_BOOT_CONFIG_MAX_ENTRIES];
    size_t entry_count;
    BolunBootMode default_mode;
    uint32_t boot_timeout_milliseconds;
} BolunBootConfiguration;

typedef enum BolunBootManagerStatus {
    BOLUN_BOOT_MANAGER_OK = 0,
    BOLUN_BOOT_MANAGER_BAD_ARGUMENT = 1,
    BOLUN_BOOT_MANAGER_NO_ENABLED_ENTRY = 2,
    BOLUN_BOOT_MANAGER_INVALID_ENTRY = 3
} BolunBootManagerStatus;

void BolunBootConfigurationInitialize(BolunBootConfiguration *configuration);
BolunBootManagerStatus BolunBootConfigurationAddEntry(BolunBootConfiguration *configuration,
                                                      BolunBootEntry entry);
BolunBootManagerStatus BolunBootSelectEntry(const BolunBootConfiguration *configuration,
                                            BolunBootMode requested_mode,
                                            const BolunBootEntry **selected_entry);

#ifdef __cplusplus
}
#endif

#endif
