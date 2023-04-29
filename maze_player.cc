#include "maze_player.h"

#include <cmath>

#include "config.h"

MazePlayer::MazePlayer() :
  x_(Warehouse::pixel_width() / 2), y_(Warehouse::pixel_height() - 8),
  facing_(Direction::North),
  state_(State::Waiting),
  sprites_("maze_player.png", 4, 16, 24),
  timer_(0) {}

void MazePlayer::move(Direction dir) {
  state_ = State::Walking;
  facing_ = dir;
}

void MazePlayer::stop() {
  state_ = State::Waiting;
}

void MazePlayer::update(const Warehouse& warehouse, unsigned int elapsed) {
  if (state_ == State::Walking) {
    const double delta = kSpeed * elapsed;
    double dx = 0, dy = 0;
    switch (facing_) {
      case Direction::North: dy = -delta; break;
      case Direction::East:  dx = delta;  break;
      case Direction::South: dy = delta;  break;
      case Direction::West:  dx = -delta; break;
    }

    if (move_if_possible(warehouse, dx, dy)) {
      timer_ = (timer_ + elapsed) % (kAnimationTime * 4);
    }
  }
}

void MazePlayer::draw(Graphics& graphics, int xo, int yo) const {
  const int x = x_ - xo - Config::kTileSize / 2;
  const int y = y_ - yo - Config::kTileSize;
  sprites_.draw(graphics, sprite_number(), x, y);

#ifndef NDEBUG
  collision_box().draw(graphics, xo, yo, 0xd8ff00ff, false);
#endif
}

Rect MazePlayer::draw_box() const {
  return {
    x_ - Config::kTileSize / 2,
    y_ - Config::kTileSize / 2,
    x_ + Config::kTileSize / 2,
    y_ + Config::kTileSize / 2 };
}

Rect MazePlayer::collision_box() const {
  return {
    x_ - Config::kTileSize / 2 + 2,
    y_ - Config::kTileSize / 4,
    x_ + Config::kTileSize / 2 - 2,
    y_ + Config::kTileSize / 2 - 2 };
}

void MazePlayer::set_position(int x, int y) {
  x_ = x;
  y_ = y;
  stop();
}

bool MazePlayer::move_if_possible(const Warehouse& warehouse, double dx, double dy) {
  x_ += dx;
  y_ += dy;

  if (collision(warehouse)) {
    x_ -= dx;
    y_ -= dy;
    return false;
  } else {
    return true;
  }
}

bool MazePlayer::collision(const Warehouse& warehouse) const {
  return !warehouse.box_walkable(collision_box());
}

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

int MazePlayer::sprite_number() const {
  int d = 4 * facing_;
  if (state_ == State::Walking) {
    d += timer_ / kAnimationTime;
  }
  return d;
}

