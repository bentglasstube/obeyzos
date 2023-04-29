#pragma once

#include "spritemap.h"

#include "maze_char.h"
#include "warehouse.h"

class MazePlayer : public MazeChar {
  public:

    MazePlayer();

    Warehouse::Cell interact(const Warehouse& warehouse) const;

  private:

    SpriteMap sprites_;
};
