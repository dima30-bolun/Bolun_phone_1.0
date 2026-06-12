#ifndef BOLUN_GRAPHICS_TILE_ENGINE_HPP
#define BOLUN_GRAPHICS_TILE_ENGINE_HPP

#include <cstdint>

namespace bolun::graphics {

enum class TileState : std::uint8_t {
    static_tile = 0,
    live_tile = 1,
    notification_tile = 2,
};

struct TileMetrics {
    std::uint32_t width;
    std::uint32_t height;
    std::uint32_t margin;
};

struct TileLayoutResult {
    std::uint32_t x;
    std::uint32_t y;
    std::uint32_t width;
    std::uint32_t height;
};

class TileEngine final {
public:
    explicit constexpr TileEngine(TileMetrics metrics) : metrics_(metrics) {}

    [[nodiscard]] constexpr TileLayoutResult layout(std::uint32_t index, std::uint32_t columns) const
    {
        const std::uint32_t safe_columns = columns == 0U ? 1U : columns;
        const std::uint32_t column = index % safe_columns;
        const std::uint32_t row = index / safe_columns;
        return TileLayoutResult{
            column * (metrics_.width + metrics_.margin),
            row * (metrics_.height + metrics_.margin),
            metrics_.width,
            metrics_.height,
        };
    }

private:
    TileMetrics metrics_;
};

} // namespace bolun::graphics

#endif
