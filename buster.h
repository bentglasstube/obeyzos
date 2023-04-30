#pragma once

#include <random>

#include "maze_char.h"
#include "maze_player.h"

class Buster : public MazeChar {
  public:
    Buster(unsigned long seed, std::pair<int, int> tile_pos);

    void update(const MazePlayer& player, const Warehouse& warehouse, unsigned int elapsed);

  private:

    double speed() const override { return visible_ ? 0.085 : 0.045; }
    int sprite_base() const override { return visible_ ? 48 : 64; }
    int wander_time() { return std::uniform_int_distribution<int>(500, 10000)(rng_); }

    std::mt19937 rng_;
    bool visible_;
    int wander_timer_;

    void chase(const MazePlayer& player);
};
