#include "worker.h"

Worker::Worker(unsigned long seed, double x, double y) :
  MazeChar(x, y),
  sprites_("worker.png", 4, 16, 24),
  rng_(seed), wait_timer_(0),
  visual_(std::uniform_int_distribution<int>(0, 1)(rng_)),
  unionized_(false) {}

void Worker::update(const Warehouse& warehouse, unsigned int elapsed) {
  if (wait_timer_ <= 0) {
    move(Direction::random(rng_));
    wait_timer_ = walk_time();
  } else {
    wait_timer_ -= elapsed;
  }

  MazeChar::update(warehouse, elapsed);
}
