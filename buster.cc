#include "buster.h"

Buster::Buster(unsigned long seed, std::pair<int, int> tile_pos) :
  MazeChar(tile_pos), rng_(seed) {}

void Buster::update(const Warehouse& warehouse, unsigned int elapsed) {
  if (!chasing_) {
    if (wander_timer_ <= 0) {
      move(Direction::random(rng_));
      wander_timer_ = wander_time();
    } else {
      wander_timer_ -= elapsed;
    }
  }

  MazeChar::update(warehouse, elapsed);
}

void Buster::chase(const MazePlayer& player) {
  chasing_ = true;

  double dx = player.x() - x();
  double dy = player.y() - y();

  // TODO better pathing
  if (std::abs(dx) > std::abs(dy)) {
    move(dx > 0 ? Direction::East : Direction::West);
  } else {
    move(dy > 0 ? Direction::South : Direction::North);
  }
}

void Buster::stop_chasing() {
  if (chasing_) {
    chasing_ = false;
    wander_timer_ = 0;
  }
}
