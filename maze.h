#pragma once

#include <bitset>
#include <random>
#include <stack>
#include <vector>

#include "direction.h"

class Maze {

  public:

    typedef std::bitset<4> Cell;
    struct Point { int x, y; };

    Maze(unsigned long seed, int width, int height);

    void step();
    void generate() { while (!done()) { step(); } }
    bool done() const { return frontier_.empty() && breakup_ == 0; }

    bool wall(Point p, Direction dir) const;

    bool valid(Point p) const { return p.x >= 0 && p.x < width_ && p.y >= 0 && p.y < height_; }
    int width() const { return width_; }
    int height() const { return height_; }

    Point random_pos();
    Point center_pos() const { return { width_ / 2, height_ / 2 }; }

  private:

    std::mt19937 rng_;
    int width_, height_, breakup_;
    std::vector<Cell> cells_;
    std::stack<Point> frontier_;

    uint8_t at(Point p) const { return valid(p) ? cells_[p.y * width_ + p.x].to_ulong() : 31; }

    std::vector<Point> unvisited_neighbors(Point p) const;

    void open_wall(Point a, Point b);
    void unset(Point p, uint8_t bit) { if (valid(p)) cells_[p.y * width_ + p.x].reset(bit); }

};
