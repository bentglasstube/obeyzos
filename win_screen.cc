#include "win_screen.h"

#include "title_screen.h"

WinScreen::WinScreen(GameState gs) :
  gs_(gs),
  rocket_("rocket.png", 0, 0, 192, 112),
  fire_("fire.png", 1, 96, 64),
  timer_(0), story_index_(0)
{}

bool WinScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (dialog_) {
    dialog_.update(input, elapsed);
  } else if (story_index_ < kStoryText.size()) {
    dialog_.set_message(kStoryText[story_index_]);
    ++story_index_;
    if (story_index_ == 4) audio.play_sample("rocket.wav");
  } else {
    return false;
  }

  timer_ += elapsed;
  return true;
}

void WinScreen::draw(Graphics& graphics) const {
  fire_.draw(graphics, (timer_ / 120) % 3, 66, 205);
  rocket_.draw(graphics, 128, 128);
  if (dialog_) dialog_.draw(graphics, false);
}

Screen* WinScreen::next_screen() const {
  return new TitleScreen();
}

const std::vector<std::string> WinScreen::kStoryText = {
  //-----------------------------------------//
  "After a long legal battle, the workers\n"
  "were able to negotiate with NILE on\n"
  "improved working conditions.",

  "However, GEOFF BOZOS was very unhappy\n"
  "with this result.  He felt as if he\n"
  "were being robbed despite his piles\n"
  "of money.",

  "Bitter about his workers unionizing and\n"
  "cutting into his inflated wealth and ego,\n"
  "He decided to cut ties with the company\n"
  "and the entire planet.",

  "GEOFF and some friends got on his oddly\n"
  "shaped rocket and set off in search of\n"
  "new people to exploit.",

  "No one on earth ever saw him again.\n"
  "                                       \n"
  "                                       \n"
  "And no one cared...",
};
