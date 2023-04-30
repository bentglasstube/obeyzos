#include "bar.h"

Bar::Bar(double max, int width) : sprites_("bars.png", 9, 2, 8), max_(max), width_(width) {}

void Bar::draw(Graphics& graphics, int x, int y, double value) const {
  const int f = std::floor(width_ * value / max_);

  sprites_.draw(graphics, f == 0 ? 0 : f == 1 ? 1 : 2, x, y);

  for (int i = 2; i < width_ - 2; i += 2) {
    sprites_.draw(graphics, f < i ? 5 : f == i ? 4 : 3, x + i, y);
  }
  sprites_.draw(graphics, f == width_ ? 8 : f == width_ - 1 ? 7 : 6, x + width_ - 2, y);
}
