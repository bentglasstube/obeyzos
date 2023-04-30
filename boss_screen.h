#pragma once

#include <random>
#include <vector>

#include "rect.h"
#include "screen.h"
#include "spritemap.h"
#include "text.h"

#include "bar.h"
#include "bozos.h"
#include "direction.h"
#include "game_state.h"

class BossScreen : public Screen {
  public:

    BossScreen(GameState gs);

    bool update(const Input&, Audio&, unsigned int) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;

  private:

    static constexpr double kBulletSpeed = 50.0;
    static constexpr int kNegotiationTick = 100;

    struct Bullet {
      enum class Color { Copper, Silver, Gold, Emerald };

      Bozos::Point pos;
      double vx, vy;
      Color color;
      int timer = 0;

      void update(unsigned int elapsed);
    };

    class Wave {
      public:

        Wave(int delay, int ammo) : delay_(delay), timer_(0), ammo_(ammo) {}
        virtual ~Wave() {}
        bool ready() const { return timer_ > delay_ && ammo_ > 0; }
        bool done() const { return ammo_ == 0; }
        void update(unsigned int elapsed) { timer_ += elapsed; }
        virtual Bullet fire(const Bozos& bozos) = 0;

      protected:
        const int delay_;
        int timer_, ammo_;

        void fired() { --ammo_; timer_ -= delay_; }
    };

    class CopperWave : public Wave {
      // sprays from nose
      public:
        CopperWave(int count, double spread, unsigned long seed) : Wave(10, count), spread_(spread), rng_(seed) {}
        Bullet fire(const Bozos& bozos);

      private:
        const double spread_;
        std::mt19937 rng_;
    };

    class SilverWave : public Wave {
      // shoots randomly out of alternating ears
      public:
        SilverWave(int count, unsigned long seed) : Wave(100, count), side_(false), rng_(seed) {}
        Bullet fire(const Bozos& bozos);
      private:
        bool side_;
        std::mt19937 rng_;
    };

    class GoldWave : public Wave {
      // shoots "lasers" from eyes
      public:
        GoldWave(int count) : Wave(40, count), side_(false) {}
        Bullet fire(const Bozos& bozos);
      private:
        bool side_;
    };

    class EmeraldWave : public Wave {
      // shoots spirals from mouth
      public:
        EmeraldWave(int count, double rotation) : Wave(100, count), rotation_(rotation), angle_(0) {}
        Bullet fire(const Bozos& bozos);
      private:
        const double rotation_;
        double angle_;
    };

    class Player {
      public:
        Player();

        bool hit(const Bullet& bullet) const { return hit_box().contains(bullet.pos.x, bullet.pos.y); }
        bool invulnerable() const { return iframes_ > 0; }

        void update(unsigned int elapsed);
        void move(Direction direction);
        void stop();
        void draw(Graphics& graphics) const;

        void hurt();

        Rect hit_box() const { return { x_ - 5, y_ - 17, x_ + 6, y_ - 3 }; }

      private:

        static constexpr double kSpeed = 0.1;
        static constexpr int kAnimationTime = 120;

        double x_, y_;
        bool moving_;
        Direction facing_;
        int iframes_, timer_;

        SpriteMap sprites_;
    };

    GameState gs_;
    std::mt19937 rng_;
    Text text_;
    Bar bar_;
    SpriteMap coins_;

    Bozos bozos_;
    std::vector<Bullet> bullets_;
    std::vector<Wave*> waves_;
    Player player_;

    double negotiations_;
    int neg_timer_;
};
