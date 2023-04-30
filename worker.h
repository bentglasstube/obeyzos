#pragma once

#include <random>

#include "maze_char.h"

class Worker : public MazeChar {
  public:

    Worker(unsigned long seed, std::pair<int, int> tile_pos);

    void update(const Warehouse& warehouse, unsigned int elapsed) override;
    void unionize() { unionized_ = true; }
    void leave_union() { unionized_ = false; }
    bool is_unionized() const { return unionized_; }

  private:

    double speed() const override { return 0.025; }
    int sprite_base() const override { return 16 + 16 * visual_; }
    int wander_time() { return std::uniform_int_distribution<int>(1000, 5000)(rng_); }

    std::mt19937 rng_;
    int wander_timer_;
    const int visual_;
    bool unionized_;
};
