#include "maze.h"

#include <algorithm>

#include "util.h"

Maze::Maze(unsigned long seed, int width, int height) : rng_(seed), width_(width), height_(height) {
  cells_.reserve(width * height);
  for (int i = 0; i < width * height; ++i) {
    cells_.push_back(15);
  }

  std::uniform_int_distribution<int> rx(0, width_ - 1);
  std::uniform_int_distribution<int> ry(0, height_ - 1);

  breakup_ = width * height / 5;

  // start at a random position in the maze
  frontier_.push({rx(rng_), ry(rng_)});
}

void Maze::step() {
  while (!frontier_.empty()) {
    auto p = frontier_.top();

    std::vector<Point> neighbors = unvisited_neighbors(p);

    if (neighbors.size() < 2) frontier_.pop();

    if (neighbors.size() > 0) {
      std::uniform_int_distribution<size_t> d(0, neighbors.size() - 1);

      auto n = neighbors[d(rng_)];
      open_wall(p, n);
      frontier_.push(n);
      return;
    }
  }

  if (breakup_ > 0) {
    std::uniform_int_distribution<int> rx(1, width_ - 2);
    std::uniform_int_distribution<int> ry(1, height_ - 2);
    std::uniform_int_distribution<int> rd(0, 3);

    const int x = rx(rng_);
    const int y = ry(rng_);
    const int d = rd(rng_);

    unset({x, y}, d);
    switch (d) {
      case 0: unset({x, y - 1}, 2); break;
      case 1: unset({x + 1, y}, 3); break;
      case 2: unset({x, y + 1}, 0); break;
      case 3: unset({x - 1, y}, 1); break;
    }

    --breakup_;
    return;
  }
}

bool Maze::wall(Maze::Point p, Direction dir) const {
  if (!valid(p)) return true;
  return cells_[p.y * width_ + p.x].test(dir);
}

std::vector<Maze::Point> Maze::unvisited_neighbors(Maze::Point p) const {
  std::vector<Point> neighbors;

  if (at({p.x - 1, p.y}) == 15) neighbors.push_back({p.x - 1, p.y});
  if (at({p.x + 1, p.y}) == 15) neighbors.push_back({p.x + 1, p.y});
  if (at({p.x, p.y - 1}) == 15) neighbors.push_back({p.x, p.y - 1});
  if (at({p.x, p.y + 1}) == 15) neighbors.push_back({p.x, p.y + 1});

  return neighbors;
}

void Maze::open_wall(Maze::Point a, Maze::Point b) {
  if (b.x == a.x - 1 && b.y == a.y) {
    unset(a, 3); unset(b, 1);
  } else if (b.x == a.x + 1 && b.y == a.y) {
    unset(a, 1); unset(b, 3);
  } else if (b.y == a.y - 1 && b.x == a.x) {
    unset(a, 0); unset(b, 2);
  } else if (b.y == a.y + 1 && b.x == a.x) {
    unset(a, 2); unset(b, 0);
  }
}

Maze::Point Maze::random_pos() {
  std::uniform_int_distribution<int> rx(0, width_ - 1);
  std::uniform_int_distribution<int> ry(0, height_ - 1);

  return { rx(rng_), ry(rng_) };
}
