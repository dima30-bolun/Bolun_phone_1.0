#ifndef BOLUN_SHELL_SHELL_MODEL_HPP
#define BOLUN_SHELL_SHELL_MODEL_HPP

#include <cstdint>
#include <string_view>

namespace bolun::shell {

enum class ShellSurface : std::uint8_t {
    start_screen,
    lock_screen,
    notification_center,
    settings,
    application_launcher,
};

struct ShellRoute {
    ShellSurface surface;
    std::string_view title;
    bool touch_first;
};

[[nodiscard]] constexpr ShellRoute default_route()
{
    return ShellRoute{ShellSurface::start_screen, "Bolun Start", true};
}

} // namespace bolun::shell

#endif
