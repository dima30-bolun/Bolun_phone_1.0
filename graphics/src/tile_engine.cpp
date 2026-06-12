#include "bolun/graphics/tile_engine.hpp"

namespace bolun::graphics {

static_assert(TileEngine(TileMetrics{100U, 100U, 8U}).layout(3U, 2U).x == 108U);
static_assert(TileEngine(TileMetrics{100U, 100U, 8U}).layout(3U, 2U).y == 108U);

} // namespace bolun::graphics
