#pragma once

#include "graphics.h"
#include "sprite.h"
#include "spritemap.h"

class Bozos {
  public:

    struct Point { double x, y; };

    Bozos();

    void update(unsigned int elapsed);
    void draw(Graphics& graphics) const;

    Point pos() const { return pos_; }
    Point mouth() const { return { pos_.x + 76, pos_.y + 154 }; }
    Point nose() const { return { pos_.x + 66, pos_.y + 113 }; }
    Point eye(bool left) const {
      return { pos_.x + (double)(left ? 39 : 98), pos_.y + (double)(left ? 96 : 89) };
    }
    Point ear(bool left) const {
      return { pos_.x + (double)(left ? 14 : 138), pos_.y + (double)(left ? 119 : 107) };
    }

    void open_mouth() { mouth_open_ = true; }
    void close_mouth() { mouth_open_ = true; }

    bool attack_ready() const { return attack_timer_ <= 0; }
    void reset_attack_cooldown() { attack_timer_  = 10000; }

  private:

    static constexpr int kHalfWidth = 128;

    Sprite face_;
    Sprite shine_;
    Sprite eyes_;

    SpriteMap eyebrows_;
    SpriteMap mouth_;

    Point pos_;
    int timer_, attack_timer_;
    bool mouth_open_;

    int mouth_sprite() const {
      if (mouth_open_) return 2;
      return attack_timer_ < 500 ? 0 : 1;
    }
};
