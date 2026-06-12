#include <cassert>

#include "bolun/graphics/tile_engine.hpp"
#include "bolun/mainos/launch_plan.hpp"
#include "bolun/services/service_registry.hpp"
#include "bolun/shell/shell_model.hpp"

int main()
{
    const bolun::graphics::TileEngine engine({120U, 120U, 12U});
    const auto result = engine.layout(5U, 2U);
    assert(result.x == 132U);
    assert(result.y == 264U);

    const auto *package_service = bolun::services::find_service(bolun::services::ServiceClass::package);
    assert(package_service != nullptr);
    assert(package_service->critical);

    const auto route = bolun::shell::default_route();
    assert(route.touch_first);

    static_assert(bolun::mainos::kMainOsLaunchPlan.size() == 5U);
    static_assert(bolun::mainos::kMainOsLaunchPlan[4].component == "bolun-shell");
    return 0;
}
