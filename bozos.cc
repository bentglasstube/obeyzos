#include "bozos.h"

#include "config.h"

Bozos::Bozos() :
  face_("bozos-face.png", 0, 0, 160, 192),
  shine_("bozos-shine.png", 0, 0, 51, 29),
  eyes_("bozos-eyes.png", 0, 0, 94, 25),
  eyebrows_("bozos-eyebrows.png", 1, 96, 19),
  mouth_("bozos-mouth.png", 1, 67, 30)
{}

void Bozos::update(unsigned int elapsed) {
  timer_ += elapsed;

  const double t = timer_ / 1000.0;
  const double cos = std::cos(t);
  const double sin = std::sin(t);
  const double denom = 1 + sin * sin;

  x_ = 176 + kHalfWidth * cos / denom;
  y_ = 50 + kHalfWidth * sin * cos / denom;
}

void Bozos::draw(Graphics& graphics) const {
  eyes_.draw(graphics, x_ + 24, y_ + 80);
  face_.draw(graphics, x_, y_);
  shine_.draw(graphics, x_ + 28, y_ + 12);

  eyebrows_.draw(graphics, 0, x_ + 23, y_ + 73);
  mouth_.draw(graphics, 0, x_ + 43, y_ + 139);
}
