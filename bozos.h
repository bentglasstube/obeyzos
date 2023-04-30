#pragma once

#include "graphics.h"
#include "sprite.h"
#include "spritemap.h"

class Bozos {
  public:

    Bozos();

    void update(unsigned int elapsed);
    void draw(Graphics& graphics) const;

    double x() const { return x_; }
    double y() const { return y_; }

  private:

    static constexpr int kHalfWidth = 128;

    Sprite face_;
    Sprite shine_;
    Sprite eyes_;

    SpriteMap eyebrows_;
    SpriteMap mouth_;

    double x_, y_;
    int timer_;
};
