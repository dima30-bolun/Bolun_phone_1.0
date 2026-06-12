#ifndef BOLUN_MAINOS_LAUNCH_PLAN_HPP
#define BOLUN_MAINOS_LAUNCH_PLAN_HPP

#include <array>
#include <cstdint>
#include <string_view>

namespace bolun::mainos {

struct LaunchStep {
    std::string_view component;
    std::uint8_t order;
    bool required;
};

constexpr std::array<LaunchStep, 5> kMainOsLaunchPlan{{
    {"kernel-object-manager", 10U, true},
    {"driver-manager", 20U, true},
    {"package-service", 30U, true},
    {"graphics-compositor", 40U, true},
    {"bolun-shell", 50U, true},
}};

} // namespace bolun::mainos

#endif
