#ifndef BOLUN_RECOVERY_RECOVERY_PLAN_H
#define BOLUN_RECOVERY_RECOVERY_PLAN_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum BolunRecoveryAction {
    BOLUN_RECOVERY_ACTION_VALIDATE_BOOT = 1,
    BOLUN_RECOVERY_ACTION_VALIDATE_STORAGE = 2,
    BOLUN_RECOVERY_ACTION_APPLY_UPDATE = 3,
    BOLUN_RECOVERY_ACTION_REPAIR_MAINOS = 4
} BolunRecoveryAction;

#ifdef __cplusplus
}
#endif

#endif
