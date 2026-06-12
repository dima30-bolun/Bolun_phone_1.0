#include "bolun/services/service_registry.hpp"

namespace bolun::services {

static_assert(find_service(ServiceClass::package) != nullptr);
static_assert(find_service(ServiceClass::account)->critical == false);

} // namespace bolun::services
