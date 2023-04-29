#pragma once

#include "spritemap.h"

#include "maze_char.h"
#include "warehouse.h"

class MazePlayer : public MazeChar {
  public:

    MazePlayer();

    Warehouse::Cell interact(const Warehouse& warehouse) const;

  private:

    static constexpr double kSpeed = 0.075;
    double speed() const override { return kSpeed; }

    SpriteMap sprites_;
    SpriteMap sprites() const override { return sprites_; }
};
