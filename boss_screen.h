#pragma once

#include <random>
#include <string>
#include <vector>

#include "rect.h"
#include "screen.h"
#include "spritemap.h"
#include "text.h"

#include "bar.h"
#include "bozos.h"
#include "dialog.h"
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
    static const std::vector<std::string> kIntroText;

    struct Bullet {
      enum class Color { Copper, Silver, Gold, Emerald };

      Bozos::Point pos;
      double vx, vy;
      Color color;
      int timer = 0;

      void update(unsigned int elapsed);
      bool out_of_bounds() const;
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
        CopperWave(unsigned long seed) :
          Wave(5, 50),
          rng_(seed),
          spread_(std::uniform_real_distribution<double>(0.738, 1.232)(rng_)) {}
        Bullet fire(const Bozos& bozos);

      private:
        std::mt19937 rng_;
        const double spread_;
    };

    class SilverWave : public Wave {
      // shoots randomly out of alternating ears
      public:
        SilverWave(unsigned long seed) : Wave(100, 100), rng_(seed), side_(false) {}
        Bullet fire(const Bozos& bozos);
      private:
        std::mt19937 rng_;
        bool side_;
    };

    class GoldWave : public Wave {
      // shoots "lasers" from eyes
      public:
        GoldWave() : Wave(40, 250), side_(false) {}
        Bullet fire(const Bozos& bozos);
      private:
        bool side_;
    };

    class EmeraldWave : public Wave {
      // shoots spirals from mouth
      public:
        EmeraldWave(unsigned long seed) :
          Wave(100, 200),
          rng_(seed),
          rotation_(std::uniform_real_distribution<double>(0.173, 0.291)(rng_)),
          angle_(0) {}
        Bullet fire(const Bozos& bozos);
      private:
        std::mt19937 rng_;
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

    Dialog dialog_;
    Bozos bozos_;
    std::vector<Bullet> bullets_;
    std::vector<Wave*> waves_;
    Player player_;

    double negotiations_;
    int neg_timer_;
    size_t dialog_index_;
};
