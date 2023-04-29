#include "worker.h"

Worker::Worker(unsigned long seed, double x, double y) :
  MazeChar(x, y),
  sprites_("worker.png", 4, 16, 24),
  rng_(seed), tx_(0), ty_(0), wait_timer_(0),
  visual_(std::uniform_int_distribution<int>(0, 1)(rng_)) {}

void Worker::update(const Warehouse& warehouse, unsigned int elapsed) {
  if (wait_timer_ <= 0) {
    move(Direction::random(rng_));
    wait_timer_ = walk_time();
  } else {
    wait_timer_ -= elapsed;
  }

  MazeChar::update(warehouse, elapsed);
}
