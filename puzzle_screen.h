#pragma once

#include <array>
#include <random>

#include "screen.h"

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

    std::array<int, 12> sequence_;
    State state_;
    int floor_, strikes_;
    int timer_, index_;
};
