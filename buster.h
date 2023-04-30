#pragma once

#include <random>

#include "maze_char.h"
#include "maze_player.h"

class Buster : public MazeChar {
  public:
    Buster(unsigned long seed, std::pair<int, int> tile_pos);

    void update(const Warehouse& warehouse, unsigned int elapsed) override;
    void chase(const MazePlayer& player);
    void stop_chasing();

  private:

    double speed() const override { return chasing_ ? 0.085 : 0.045; }
    int sprite_base() const override { return chasing_ ? 48 : 64; }
    int wander_time() { return std::uniform_int_distribution<int>(500, 10000)(rng_); }

    std::mt19937 rng_;
    bool chasing_;
    int wander_timer_;
};
