#include "worker.h"

Worker::Worker(unsigned long seed, std::pair<int, int> tile_pos) :
  MazeChar(tile_pos),
  rng_(seed), wander_timer_(0),
  visual_(std::uniform_int_distribution<int>(0, 1)(rng_)),
  unionized_(false) {}

void Worker::update(const Warehouse& warehouse, unsigned int elapsed) {
  if (wander_timer_ <= 0) {
    move(Direction::random(rng_));
    wander_timer_ = wander_time();
  } else {
    wander_timer_ -= elapsed;
  }
  MazeChar::update(warehouse, elapsed);
}
