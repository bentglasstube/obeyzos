#pragma once

#include "maze_char.h"
#include "warehouse.h"

class MazePlayer : public MazeChar {
  public:

    MazePlayer();

    Warehouse::Cell interact(const Warehouse& warehouse) const;
    Rect target() const;

  private:

    double speed() const override { return 0.075; }
    int sprite_base() const override { return 0; }
};
