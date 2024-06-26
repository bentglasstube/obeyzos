#pragma once

#include <array>
#include <random>

#include "graphics.h"
#include "rect.h"
#include "spritemap.h"

#include "config.h"

class Warehouse {
  public:

    class Tile {
      public:
        enum Value : uint8_t {
          Open,
          Wall, WallFace, Shelf,
          Elevator, Entrance,
          OOB
        };

        constexpr Tile() : value_(Wall) {}
        constexpr Tile(Value v) : value_(v) {}
        operator Value() const { return value_; }
        explicit operator bool() = delete;

        bool obstructs() const {
          switch (value_) {
            case Open:
            case Entrance:
              return false;
            default:
              return true;
          }
        }

        bool occludes() const {
          switch (value_) {
            case Open:
            case WallFace:
            case Shelf:
            case Elevator:
            case Entrance:
              return false;
            default:
              return true;
          }
        }

      private:
        Value value_;
    };

    struct Cell {
      Tile tile = Tile::OOB;
      bool visible = false, seen = false;
      int sprite = 0;
    };

    Warehouse(unsigned long seed);

    void draw(Graphics& graphics, int xo, int yo, bool show_all=false) const;
    void generate(unsigned long seed);

    bool box_walkable(const Rect& r, double dx, double dy) const;
    bool box_walkable(const Rect& r) const { return box_walkable(r, 0, 0); }
    bool walkable(double px, double py) const;
    bool box_visible(const Rect& r) const;

    void calculate_visibility(int x, int y);
    Tile get_tile(int x, int y) const;
    Cell get_cell(int x, int y) const;

    template <class Generator> std::pair<int, int> random_open_cell(Generator& g) const {
      std::uniform_int_distribution<int> rx(1, kMapWidth - 2);
      std::uniform_int_distribution<int> ry(2, kMapHeight - 2);

      while (true) {
        int x = rx(g);
        int y = ry(g);

        if (get_tile(x, y) == Tile::Open) {
          return std::make_pair(x, y);
        }
      }
    }

    static constexpr int pixel_width() { return kMapWidth * Config::kTileSize; }
    static constexpr int pixel_height() { return kMapHeight * Config::kTileSize; }

    static constexpr int kMazeSize = 15;
    static constexpr int kMapWidth = 3 * kMazeSize + 1;
    static constexpr int kMapHeight = 4 * kMazeSize + 1;

  private:

    static constexpr int kVisibility = 11;

    SpriteMap tiles_;
    std::mt19937 rng_;
    std::array<Cell, kMapHeight * kMapWidth> cells_;

    struct Shadow {
      double start, end;
      bool contains(const Shadow& other) const;
    };

    class ShadowLine {
      public:
        ShadowLine() {}
        bool is_shadowed(const Shadow& shadow) const;
        void add(const Shadow& shadow);

      private:
        std::vector<Shadow> shadows_;
    };

    int index(int x, int y) const { return y * kMapWidth + x; }
    void set_tile(int x, int y, Tile t);
    void set_visible(int x, int y, bool visible);

    bool match_or_oob(int x, int y, Tile match) const;
    void make_elevator();
    void make_entrance();
    int sprite_id(Tile tile, int neighbors) const;
    void pick_sprite(int x, int y);
    void pick_all_sprites();
};
