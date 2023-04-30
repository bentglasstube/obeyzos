#include "boss_screen.h"

BossScreen::BossScreen(GameState gs) : gs_(gs) {}

bool BossScreen::update(const Input&, Audio&, unsigned int elapsed) {
  bozos_.update(elapsed);
  return true;
}

void BossScreen::draw(Graphics& graphics) const {
  bozos_.draw(graphics);
}

Screen* BossScreen::next_screen() const {
  return nullptr;
}
