#pragma once

#include "graphics.h"
#include "rect.h"
#include "spritemap.h"

#include "direction.h"
#include "warehouse.h"

class MazeChar {
  public:

    enum class State { Waiting, Walking };

    MazeChar(double x, double y);
    MazeChar(std::pair<int, int> tile_pos);
    virtual ~MazeChar() {}

    void move(Direction dir);
    void stop();

    virtual void update(const Warehouse& warehouse, unsigned int elapased);
    virtual void draw(Graphics& graphics, int xo, int yo) const;

    Rect draw_box() const;
    Rect collision_box() const;

    double x() const { return x_; }
    double y() const { return y_; }

    void set_position(double x, double y);
    void set_tile_position(std::pair<int, int> tile_position);

  protected:

    static constexpr int kAnimationTime = 125;

    double x_, y_;
    Direction facing_;
    State state_;
    SpriteMap sprites_;
    int timer_;

    bool collision(const Warehouse& warehouse, double dx, double dy) const;
    int sprite_number() const;

    virtual double speed() const = 0;
    virtual int sprite_base() const = 0;
};
