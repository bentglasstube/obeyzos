#pragma once

#include <random>

#include "maze_char.h"

class Worker : public MazeChar {
  public:

    Worker(unsigned long seed, double x, double y);

    void update(const Warehouse& warehouse, unsigned int elapsed) override;
    void unionize() { unionized_ = true; }
    bool is_unionized() const { return unionized_; }

  private:

    static constexpr int kMinWalkTime = 1000;
    static constexpr int kMaxWalkTime = 5000;

    double speed() const override { return 0.025; }
    int sprite_base() const override { return 16 + 16 * visual_; }

    int walk_time() { return std::uniform_int_distribution<int>(kMinWalkTime, kMaxWalkTime)(rng_); }

    std::mt19937 rng_;
    int wait_timer_;
    const int visual_;
    bool unionized_;
};
