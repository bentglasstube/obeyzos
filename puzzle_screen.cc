#include "puzzle_screen.h"

#include "boss_screen.h"

PuzzleScreen::PuzzleScreen(GameState gs) :
  gs_(gs), rng_(gs.seed),
  text_("text.png"),
  background_("puzzle-background.png"),
  lights_("puzzle-lights.png", 5, 144, 144),
  digits_("puzzle-digits.png", 11, 20, 37),
  crosses_("puzzle-strikes.png", 4, 13, 41),
  state_(State::Intro),
  floor_(1), strikes_(0), index_(0),
  timer_(0), playing_(0), playing_timer_(0)
{
  make_random_sequence();
}

bool PuzzleScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  gs_.add_time(elapsed);

#ifndef NDEBUG
  if (input.key_pressed(Input::Button::Select)) floor_ = 20;
#endif

  if (dialog_) {
    dialog_.update(input, elapsed);
    return true;
  }

  if (state_ == State::Intro) {
    dialog_.set_message("Looks like the badge reader is broken.  I'll\nhave to use the \"foolproof\" obedience\nbased security lock instead.");
    playback_mode();
  } else if (state_ == State::Playback) {

    timer_ -= elapsed;
    if (timer_ < 0) {
      play_note(audio, sequence_[index_]);
      ++index_;
      if (index_ == floor_) {
        input_mode();
      } else {
        timer_ = 1000;
      }
    }

  } else {

    timer_ -= elapsed;
    if (timer_ < 0) check_note(audio, 0);

    if (input.key_pressed(Input::Button::Up)) {
      check_note(audio, 1);
    } else if (input.key_pressed(Input::Button::Right)) {
      check_note(audio, 2);
    } else if (input.key_pressed(Input::Button::Down)) {
      check_note(audio, 3);
    } else if (input.key_pressed(Input::Button::Left)) {
      check_note(audio, 4);
    }
  }

  if (playing_timer_ > 0) playing_timer_ -= elapsed;

  if (floor_ > sequence_.size()) {
    audio.play_sample("simon-winner.wav");
    return false;
  }

  return true;
}

void PuzzleScreen::draw(Graphics& graphics) const {
  background_.draw(graphics);

  crosses_.draw(graphics, strikes_, 370, 46);
  digits_.draw(graphics, (floor_ > 9 ? 1 : 10), 390, 49);
  digits_.draw(graphics, floor_ % 10, 422, 49);
  lights_.draw(graphics, playing_timer_ > 0 ? playing_ : 0, 176, 147);

  text_.draw(graphics, "Workers: " + std::to_string(gs_.workers), 0, 0);
  text_.draw(graphics, gs_.clock(), graphics.width(), 0, Text::Alignment::Right);

  if (dialog_) dialog_.draw(graphics);
}

Screen* PuzzleScreen::next_screen() const {
  return new BossScreen(gs_);
}

void PuzzleScreen::play_note(Audio& audio, int note) {
  playing_ = note;
  playing_timer_ = 750;
  audio.play_sample("simon" + std::to_string(note) + ".wav");
}

void PuzzleScreen::input_mode() {
  state_ = State::Input;
  timer_ = 3000;
  index_ = 0;
}

void PuzzleScreen::playback_mode() {
  state_ = State::Playback;
  timer_ = 1500;
  index_ = 0;
}

void PuzzleScreen::check_note(Audio& audio, int note) {
  if (note == sequence_[index_]) {
    play_note(audio, note);
    ++index_;
    if (index_ == floor_) {
      ++floor_;
      playback_mode();
    } else {
      timer_ = 3000;
    }
  } else {
    ++strikes_;
    if (strikes_ == 3) {
      gs_.workers--;
      strikes_ = 0;
      floor_ = 1;
      make_random_sequence();
      audio.play_sample("simon-loss.wav");
    } else {
      audio.play_sample("buzzer.wav");
    }
    playback_mode();
  }
}

void PuzzleScreen::make_random_sequence() {
  std::uniform_int_distribution<int> color(1, 4);
  for (size_t i = 0; i < sequence_.size(); ++i) {
    sequence_[i] = color(rng_);
  }
}
