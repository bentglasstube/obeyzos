#pragma once

#include "graphics.h"
#include "rect.h"
#include "spritemap.h"

#include "warehouse.h"
#include "direction.h"

class MazePlayer {
  public:

    enum class State { Waiting, Walking };

    MazePlayer();

    void move(Direction dir);
    void stop();
    Warehouse::Cell interact(const Warehouse& warehouse) const;

    void update(const Warehouse& warehouse, unsigned int elapased);
    void draw(Graphics& graphics, int xo, int yo) const;

    Rect draw_box() const;
    Rect collision_box() const;

    double x() const { return x_; }
    double y() const { return y_; }

    void set_position(int x, int y);

  private:

    static constexpr double kSpeed = 0.075;
    static constexpr int kAnimationTime = 125;

    double x_, y_;
    Direction facing_;
    State state_;
    SpriteMap sprites_;
    int timer_;

    bool move_if_possible(const Warehouse& warehouse, double dx, double dy);
    bool collision(const Warehouse& warehouse) const;
    int sprite_number() const;
};
