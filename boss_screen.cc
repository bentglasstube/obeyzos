#include "boss_screen.h"

#include <algorithm>

#include "config.h"

BossScreen::BossScreen(GameState gs) :
  gs_(gs), rng_(gs.seed),
  text_("text.png"),
  bar_(1000.0, kConfig.graphics.width),
  coins_("coins.png", 4, 16, 16),
  negotiations_(0), neg_timer_(0)
{}

bool BossScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  gs_.add_time(elapsed);
  bozos_.update(elapsed);
  for (auto& bullet : bullets_) {
    bullet.update(elapsed);

    if (player_.hit(bullet) && !player_.invulnerable()) {
      player_.hurt();
      if (--gs_.workers < 1) {
        // game over
        return false;
      }
    }
  }

  for (auto wave : waves_) {
    wave->update(elapsed);
    while (wave->ready()) {
      bullets_.emplace_back(wave->fire(bozos_));
    }
  }

  bullets_.erase(
      std::remove_if(
        bullets_.begin(), bullets_.end(),
        [](const auto& b) { return b.out_of_bounds(); }),
      bullets_.end());

  waves_.erase(
      std::remove_if(
        waves_.begin(), waves_.end(),
        [](const auto& w) { return w->done(); }),
      waves_.end());

  if (bozos_.attack_ready()) {
    int roll = std::uniform_int_distribution<int>(1, 10)(rng_);
    if (roll <= 4) {
      waves_.push_back(new CopperWave{100, 1, rng_()});
    } else if (roll <= 7) {
      waves_.push_back(new SilverWave{100, rng_()});
    } else if (roll <= 9) {
      waves_.push_back(new GoldWave{100});
    } else {
      waves_.push_back(new EmeraldWave{100, 0.2});
      bozos_.open_mouth();
    }

    bozos_.reset_attack_cooldown();
  }

  if (waves_.empty()) bozos_.close_mouth();

  if (input.key_held(Input::Button::Left)) {
    player_.move(Direction::West);
  } else if (input.key_held(Input::Button::Right)) {
    player_.move(Direction::East);
  } else if (input.key_held(Input::Button::Up)) {
    player_.move(Direction::North);
  } else if (input.key_held(Input::Button::Down)) {
    player_.move(Direction::South);
  } else {
    player_.stop();
  }

  player_.update(elapsed);

  neg_timer_ += elapsed;
  if (neg_timer_ > kNegotiationTick) {
    neg_timer_ -= kNegotiationTick;
    negotiations_ += gs_.workers / 10.0;
  }

  return true;
}

void BossScreen::draw(Graphics& graphics) const {
  bozos_.draw(graphics);
  player_.draw(graphics);
  for (const auto& bullet : bullets_) {
    coins_.draw(graphics, (int)bullet.color * 4 + (bullet.timer / 100) % 4, bullet.pos.x - 8, bullet.pos.y - 8);
  }

  text_.draw(graphics, "Workers: " + std::to_string(gs_.workers), 0, 0);
  text_.draw(graphics, gs_.clock(), graphics.width(), 0, Text::Alignment::Right);
  text_.draw(graphics, "Negotiations", graphics.width() / 2, graphics.height() - 32, Text::Alignment::Center);
  bar_.draw(graphics, 0, graphics.height() - 12, negotiations_);
}

Screen* BossScreen::next_screen() const {
  return nullptr;
}

void BossScreen::Bullet::update(unsigned int elapsed) {
  pos.x += vx * elapsed / 1000.0;
  pos.y += vy * elapsed / 1000.0;
  timer += elapsed;
}

bool BossScreen::Bullet::out_of_bounds() const {
  if (pos.x < -10 && vx < 0) return true;
  if (pos.y < -10 && vy < 0) return true;
  if (pos.x > kConfig.graphics.width + 10 && vx > 0) return true;
  if (pos.y > kConfig.graphics.height + 10 && vy > 0) return true;
  return false;
}

BossScreen::Bullet BossScreen::CopperWave::fire(const Bozos& bozos) {
  fired();

  double angle = std::uniform_real_distribution<double>(-spread_, spread_)(rng_);
  return {
    bozos.nose(),
    kBulletSpeed * 2.5 * std::sin(angle),
    kBulletSpeed * 2.5 * std::cos(angle),
    Bullet::Color::Copper
  };
}

BossScreen::Bullet BossScreen::SilverWave::fire(const Bozos& bozos) {
  fired();

  double angle = std::uniform_real_distribution<double>(0, 2 * M_PI)(rng_);
  side_ = !side_;
  return {
    bozos.ear(side_),
    kBulletSpeed * std::cos(angle),
    kBulletSpeed * std::sin(angle),
    Bullet::Color::Silver };
}

BossScreen::Bullet BossScreen::GoldWave::fire(const Bozos& bozos) {
  fired();

  side_ = !side_;
  return {
    bozos.eye(side_),
    0,
    5 * kBulletSpeed,
    Bullet::Color::Gold
  };
}

BossScreen::Bullet BossScreen::EmeraldWave::fire(const Bozos& bozos) {
  fired();

  angle_ += rotation_;
  return {
    bozos.mouth(),
    kBulletSpeed * std::cos(angle_),
    kBulletSpeed * std::sin(angle_),
    Bullet::Color::Emerald
  };
}

BossScreen::Player::Player() :
  x_(kConfig.graphics.width / 2 - 16),
  y_(kConfig.graphics.height - 64),
  sprites_("unionman.png", 3, 32, 64)
{}

void BossScreen::Player::update(unsigned int elapsed) {
  timer_ = (timer_ + elapsed) % (kAnimationTime * 3);
  if (moving_) {
    const double delta = kSpeed * elapsed;
    double dx = facing_.dx() * delta;
    double dy = facing_.dy() * delta;

    x_ += dx;
    y_ += dy;

    x_ = std::clamp(16.0, x_, kConfig.graphics.width - 16.0);
    y_ = std::clamp(240.0, y_, kConfig.graphics.height - 32.0);
  }

  if (iframes_ > 0) iframes_ -= elapsed;
}

void BossScreen::Player::move(Direction direction) {
  facing_ = direction;
  moving_ = true;
}

void BossScreen::Player::stop() {
  moving_ = false;
}

void BossScreen::Player::draw(Graphics& graphics) const {
  if ((iframes_ / 125) % 2 == 0) {
    sprites_.draw(graphics, (timer_ / kAnimationTime) % 3, x_ - 16, y_ - 32);
  }
#ifndef NDEBUG
  hit_box().draw(graphics, 0, 0, 0xd8ff00ff, false);
#endif
}

void BossScreen::Player::hurt() {
  iframes_ = 1500;
}
