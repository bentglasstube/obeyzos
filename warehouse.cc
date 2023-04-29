#include "warehouse.h"

#include "maze.h"

Warehouse::Warehouse(unsigned long seed) :
  tiles_("maze_tiles.png", 8, Config::kTileSize, Config::kTileSize)
{
  generate(seed);
}

void Warehouse::draw(Graphics& graphics, int xo, int yo) const {
  for (int y = 0; y < kMapHeight; ++y) {
    for (int x = 0; x < kMapWidth; ++x) {
      const Cell c = cells_[index(x, y)];
      const Graphics::Point a = {x * Config::kTileSize - xo, y * Config::kTileSize - yo};
      const Graphics::Point b = {a.x + Config::kTileSize, a.y + Config::kTileSize};

      if (c.seen) {
        tiles_.draw(graphics, c.sprite, a.x, a.y);
        if (!c.visible) graphics.draw_rect(a, b, 0x00000080, true);
      }
    }
  }
}

void Warehouse::generate(unsigned long seed) {
  rng_.seed(seed);

  Maze maze(rng_(), kMazeSize, kMazeSize);
  maze.generate();

  for (int y = 0; y < kMapHeight; ++y) {
    for (int x = 0; x < kMapWidth; ++x) {
      if (x == kMapWidth - 1 || y == kMapHeight - 1) {
        set_tile(x, y, Tile::Wall);
      } else {
        set_tile(x, y, Tile::Open);
      }
    }
  }

  // build shelf walls
  for (int y = 0; y < kMazeSize; ++y) {
    for (int x = 0; x < kMazeSize; ++x) {
      if (maze.wall({x, y}, Direction::North)) {
        set_tile(3 * x + 0, 4 * y, Tile::Wall);
        set_tile(3 * x + 1, 4 * y, Tile::Wall);
        set_tile(3 * x + 2, 4 * y, Tile::Wall);
        set_tile(3 * x + 3, 4 * y, Tile::Wall);
      }

      if (maze.wall({x, y}, Direction::West)) {
        set_tile(3 * x, 4 * y + 0, Tile::Wall);
        set_tile(3 * x, 4 * y + 1, Tile::Wall);
        set_tile(3 * x, 4 * y + 2, Tile::Wall);
        set_tile(3 * x, 4 * y + 3, Tile::Wall);
        set_tile(3 * x, 4 * y + 4, Tile::Wall);
      }
    }
  }

  // place elevator randomly
  const int elevator = std::uniform_int_distribution<int>(0, kMazeSize - 1)(rng_);
  set_tile(3 * elevator + 1, 1, Tile::Elevator);
  set_tile(3 * elevator + 2, 1, Tile::Elevator);

  // place shelves
  for (int y = 1; y < kMapHeight; y += 4) {
    for (int x = 1; x < kMapWidth; ++x) {
      if (get_tile(x, y) == Tile::Open && get_tile(x, y - 1) == Tile::Wall) {
        set_tile(x, y, y == 1 ? Tile::WallFace : Tile::Shelf);
      }
    }
  }

  // place entrance
  set_tile(kMapWidth / 2, kMapHeight - 1, Tile::Entrance);
  set_tile(kMapWidth / 2 - 1, kMapHeight - 1, Tile::Entrance);

  pick_all_sprites();
}

bool Warehouse::box_walkable(const Rect& r) const {
  return walkable(r.left, r.top) &&
    walkable(r.left, r.bottom) &&
    walkable(r.right, r.top) &&
    walkable(r.right, r.bottom);
}

bool Warehouse::walkable(double px, double py) const {
  const int x = std::floor(px / Config::kTileSize);
  const int y = std::floor(py / Config::kTileSize);
  return !get_tile(x, y).obstructs();
}

namespace {
  std::pair<int, int> transform(int r, int c, int octant) {
    switch (octant) {
      case 0: return { +c, -r };
      case 1: return { +r, -c };
      case 2: return { +r, +c };
      case 3: return { +c, +r };
      case 4: return { -c, +r };
      case 5: return { -r, +c };
      case 6: return { -r, -c };
      case 7: return { -c, -r };
    }

    return { 0, 0 };
  }
}

void Warehouse::calculate_visibility(int x, int y) {
  for (auto& c : cells_) {
    c.visible = false;
  }
  set_visible(x, y, true);

  for (int octant = 0; octant < 8; ++octant) {
    ShadowLine line;

    for (int r = 1; r < kVisibility; ++r) {
      for (int c = 0; c <= r; ++c) {
        Shadow s = { c / (double)(r + 1), (c + 1) / (double)r};

        const auto offset = transform(r, c, octant);
        const int cx = x + offset.first;
        const int cy = y + offset.second;

        const int dist2 = offset.first * offset.first + offset.second * offset.second;
        if (dist2 < kVisibility * kVisibility) {
          const bool visible = !line.is_shadowed(s);
          set_visible(cx, cy, visible);
          if (visible && get_tile(cx, cy).occludes()) line.add(s);
        }
      }
    }
  }
}

Warehouse::Tile Warehouse::get_tile(int x, int y) const {
  return get_cell(x, y).tile;
}

Warehouse::Cell Warehouse::get_cell(int x, int y) const {
  if (x < 0 || x >= kMapWidth) return Cell();
  if (y < 0 || x >= kMapHeight) return Cell();
  return cells_[index(x, y)];
}

void Warehouse::set_tile(int x, int y, Tile t) {
  if (x < 0 || x >= kMapWidth) return;
  if (y < 0 || x >= kMapHeight) return;

  cells_[index(x, y)].tile = t;
}

void Warehouse::set_visible(int x, int y, bool visible) {
  if (x < 0 || x >= kMapWidth) return;
  if (y < 0 || y >= kMapHeight) return;

  cells_[index(x, y)].visible = visible;
  if (visible) cells_[index(x, y)].seen = true;
}

bool Warehouse::match_or_oob(int x, int y, Tile match) const {
  const Tile t = get_tile(x, y);
  return t == match || t == Tile::OOB;
}

void Warehouse::make_elevator() {
  // TODO later
}

void Warehouse::make_entrance() {
  // TODO later
}

int Warehouse::sprite_id(Tile tile, int neighbors) const {
  switch (tile) {
    case Tile::Open:     return 4;
    case Tile::Elevator: return 0 + (neighbors & 3);
    case Tile::Entrance: return 4 + (neighbors & 3);
    case Tile::WallFace: return 8;
    case Tile::Shelf:    return 12 + (neighbors & 3);
    case Tile::Wall:     return 16 + neighbors;
    default:             return 0;
  }
}

void Warehouse::pick_sprite(int x, int y) {
  Cell& c = cells_[index(x, y)];

  const int l = match_or_oob(x - 1, y, c.tile) ? 1 : 0;
  const int r = match_or_oob(x + 1, y, c.tile) ? 2 : 0;
  const int u = match_or_oob(x, y - 1, c.tile) ? 4 : 0;
  const int d = match_or_oob(x, y + 1, c.tile) ? 8 : 0;
  c.sprite = sprite_id(c.tile, l + r + u + d);

  // spice up the walls
  std::uniform_int_distribution<int> spice(0, 3);
  if (c.tile == Tile::WallFace) {
    c.sprite += spice(rng_);
  }
}

void Warehouse::pick_all_sprites() {
  for (int y = 0; y < kMapHeight; ++y) {
    for (int x = 0; x < kMapWidth; ++x) {
      pick_sprite(x, y);
    }
  }
}

bool Warehouse::Shadow::contains(const Shadow& other) const {
  return start <= other.start && end >= other.end;
}

bool Warehouse::ShadowLine::is_shadowed(const Shadow& shadow) const {
  for (const auto& s : shadows_) {
    if (s.contains(shadow)) return true;
  }
  return false;
}

void Warehouse::ShadowLine::add(const Shadow& shadow) {
  size_t i = 0;
  for (i = 0; i < shadows_.size(); ++i) {
    if (shadows_[i].start >= shadow.start) break;
  }

  auto* prev = (i > 0 && shadows_[i - 1].end > shadow.start) ? &shadows_[i - 1] : nullptr;
  auto* next = (i < shadows_.size() && shadows_[i].start < shadow.end) ? &shadows_[i] : nullptr;

  if (next) {
    if (prev) {
      prev->end = next->end;
      shadows_.erase(shadows_.begin() + i);
    } else {
      next->start = shadow.start;
    }
  } else {
    if (prev) {
      prev->end = shadow.end;
    } else {
      shadows_.insert(shadows_.begin() + i, shadow);
    }
  }
}
