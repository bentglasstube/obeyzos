#pragma once

#include "screen.h"

#include "bozos.h"
#include "game_state.h"

class BossScreen : public Screen {
  public:

    BossScreen(GameState gs);

    bool update(const Input&, Audio&, unsigned int) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;

  private:

    GameState gs_;
    Bozos bozos_;
};
