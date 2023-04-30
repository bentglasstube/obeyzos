#include "maze_player.h"

#include <cmath>

#include "config.h"

MazePlayer::MazePlayer() : MazeChar(Warehouse::pixel_width() / 2, Warehouse::pixel_height() - 8) {}

Warehouse::Cell MazePlayer::interact(const Warehouse& warehouse) const {
  int px = std::floor(x_ / Config::kTileSize);
  int py = std::floor(y_ / Config::kTileSize);

  switch (facing_) {
    case Direction::North: --py; break;
    case Direction::East:  ++px; break;
    case Direction::South: ++py; break;
    case Direction::West:  --px; break;
  }

  return warehouse.get_cell(px, py);
}

Rect MazePlayer::target() const {
  const Rect box = draw_box();
  return Rect{
      box.left   + facing_.dx() * Config::kTileSize,
      box.top    + facing_.dy() * Config::kTileSize,
      box.right  + facing_.dx() * Config::kTileSize,
      box.bottom + facing_.dy() * Config::kTileSize,
  };
}
