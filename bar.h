#pragma once

#include "graphics.h"
#include "spritemap.h"

class Bar {
  public:
    Bar(double max, int width);
    void draw(Graphics& graphics, int x, int y, double value) const;

  private:

    SpriteMap sprites_;
    double max_, width_;
};
