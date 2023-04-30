#pragma once

#include <array>
#include <random>

#include "backdrop.h"
#include "screen.h"
#include "spritemap.h"

#include "game_state.h"

class PuzzleScreen : public Screen {
  public:

    PuzzleScreen(GameState gs);

    bool update(const Input&, Audio&, unsigned int) override;
    void draw(Graphics&) const override;

    Screen* next_screen() const override;

  private:

    enum class State { Playback, Input };

    GameState gs_;
    std::mt19937 rng_;

    Backdrop background_;
    SpriteMap lights_;
    SpriteMap digits_;
    SpriteMap crosses_;

    std::array<int, 12> sequence_;
    State state_;
    size_t floor_, strikes_, index_;
    int timer_, playing_, playing_timer_;

    void play_note(Audio&, int note);

    void input_mode();
    void playback_mode();

    void check_note(Audio& audio, int note);
    void make_random_sequence();
};
