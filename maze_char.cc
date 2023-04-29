#include "maze_char.h"

#include <cmath>

#include "config.h"

MazeChar::MazeChar(double x, double y) :
  x_(x), y_(y),
  facing_(Direction::North),
  state_(State::Waiting),
  timer_(0) {}

void MazeChar::move(Direction dir) {
  state_ = State::Walking;
  facing_ = dir;
}

void MazeChar::stop() {
  state_ = State::Waiting;
}

void MazeChar::update(const Warehouse& warehouse, unsigned int elapsed) {
  if (state_ == State::Walking) {
    const double delta = speed() * elapsed;
    double dx = 0, dy = 0;
    switch (facing_) {
      case Direction::North: dy = -delta; break;
      case Direction::East:  dx = delta;  break;
      case Direction::South: dy = delta;  break;
      case Direction::West:  dx = -delta; break;
    }

    if (!collision(warehouse, dx, dy)) {
      x_ += dx;
      y_ += dy;
      timer_ = (timer_ + elapsed) % (kAnimationTime * 4);
    }
  }
}

void MazeChar::draw(Graphics& graphics, int xo, int yo) const {
  const int x = x_ - xo - Config::kTileSize / 2;
  const int y = y_ - yo - Config::kTileSize;
  sprites().draw(graphics, sprite_number(), x, y);

#ifndef NDEBUG
  collision_box().draw(graphics, xo, yo, 0xd8ff00ff, false);
#endif
}

Rect MazeChar::draw_box() const {
  return {
    x_ - Config::kTileSize / 2,
    y_ - Config::kTileSize / 2,
    x_ + Config::kTileSize / 2,
    y_ + Config::kTileSize / 2 };
}

Rect MazeChar::collision_box() const {
  return {
    x_ - Config::kTileSize / 2 + 2,
    y_ - Config::kTileSize / 4,
    x_ + Config::kTileSize / 2 - 2,
    y_ + Config::kTileSize / 2 - 2 };
}

void MazeChar::set_position(int x, int y) {
  x_ = x;
  y_ = y;
  stop();
}

bool MazeChar::collision(const Warehouse& warehouse, double dx, double dy) const {
  return !warehouse.box_walkable(collision_box(), dx, dy);
}

int MazeChar::sprite_number() const {
  int d = 4 * facing_;
  if (state_ == State::Walking) {
    d += timer_ / kAnimationTime;
  }
  return d;
}


