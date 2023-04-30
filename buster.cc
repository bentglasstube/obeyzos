#include "buster.h"

Buster::Buster(unsigned long seed, std::pair<int, int> tile_pos) :
  MazeChar(tile_pos), rng_(seed) {}

void Buster::update(const MazePlayer& player, const Warehouse& warehouse, unsigned int elapsed) {
  visible_ = warehouse.box_visible(collision_box());

  if (visible_) {
    chase(player);
  } else if (wander_timer_ <= 0) {
    move(Direction::random(rng_));
    wander_timer_ = wander_time();
  } else {
    wander_timer_ -= elapsed;
  }

  MazeChar::update(warehouse, elapsed);
}

void Buster::chase(const MazePlayer& player) {
  double dx = player.x() - x();
  double dy = player.y() - y();

  if (std::abs(dx) > std::abs(dy)) {
    move(dx > 0 ? Direction::East : Direction::West);
  } else {
    move(dy > 0 ? Direction::South : Direction::North);
  }
}
