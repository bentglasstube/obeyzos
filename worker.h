#pragma once

#include <random>

#include "spritemap.h"

#include "maze_char.h"

class Worker : public MazeChar {
  public:

    Worker(unsigned long seed, double x, double y);

    void update(const Warehouse& warehouse, unsigned int elapsed) override;

  private:

    static constexpr int kMinWalkTime = 1000;
    static constexpr int kMaxWalkTime = 5000;
    static constexpr double kSpeed = 0.025;
    double speed() const override { return kSpeed; }

    SpriteMap sprites_;
    SpriteMap sprites() const override { return sprites_; }

    int sprite_number() const override { return 16 * visual_ + MazeChar::sprite_number(); }
    int walk_time() { return std::uniform_int_distribution<int>(kMinWalkTime, kMaxWalkTime)(rng_); }

    std::mt19937 rng_;
    double tx_, ty_;
    int wait_timer_;
    int visual_;
};
