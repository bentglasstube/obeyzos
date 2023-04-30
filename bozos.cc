#include "bozos.h"

#include "config.h"

Bozos::Bozos() :
  face_("bozos-face.png", 0, 0, 160, 192),
  shine_("bozos-shine.png", 0, 0, 51, 29),
  eyes_("bozos-eyes.png", 0, 0, 32, 16),
  eyebrows_("bozos-eyebrows.png", 1, 96, 19),
  mouth_("bozos-mouth.png", 1, 67, 30),
  pos_{0, 0}, timer_(0), attack_timer_(3500),
  mouth_open_(false)
{}

void Bozos::update(unsigned int elapsed) {
  timer_ += elapsed;
  attack_timer_ -= elapsed;

  const double t = timer_ / 1000.0;
  const double cos = std::cos(t);
  const double sin = std::sin(t);
  const double denom = 1 + sin * sin;

  pos_.x = 176 + kHalfWidth * cos / denom;
  pos_.y = 50 + kHalfWidth * sin * cos / denom;
}

void Bozos::draw(Graphics& graphics) const {
  eyes_.draw(graphics, pos_.x + 24, pos_.y + 88);
  eyes_.draw(graphics, pos_.x + 79, pos_.y + 84);
  face_.draw(graphics, pos_.x, pos_.y);
  shine_.draw(graphics, pos_.x + 28, pos_.y + 12);

  eyebrows_.draw(graphics, 0, pos_.x + 23, pos_.y + 73);
  mouth_.draw(graphics, mouth_sprite(), pos_.x + 43, pos_.y + 139);
}
