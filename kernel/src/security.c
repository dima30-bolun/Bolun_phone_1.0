#include "bolun/kernel/security.h"

BolunCapability BolunCapabilityCreate(uint32_t object_id, BolunKernelObjectType object_type, unsigned int rights)
{
    BolunCapability capability;
    capability.object_id = object_id;
    capability.object_type = object_type;
    capability.rights = rights;
    return capability;
}

bool BolunCapabilityAllows(BolunCapability capability, unsigned int requested_rights)
{
    if (capability.object_id == 0U) {
        return false;
    }

    return (capability.rights & requested_rights) == requested_rights;
}
