#include "puzzle_screen.h"

#include "boss_screen.h"

PuzzleScreen::PuzzleScreen(GameState gs) :
  gs_(gs), rng_(gs.seed),
  state_(State::Playback), floor_(1), strikes_(0),
  timer_(0), index_(0)
{
  std::uniform_int_distribution<int> color(1, 4);
  for (size_t i = 0; i < sequence_.size(); ++i) {
    sequence_[i] = color(rng_);
  }
}

bool PuzzleScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {

}

void PuzzleScreen::draw(Graphics& graphics) const {
}

Screen* PuzzleScreen::next_screen() const {
  return new BossScreen(gs_);
}
