#pragma once

#include "dialog.h"
#include "screen.h"
#include "sprite.h"
#include "spritemap.h"

#include "game_state.h"

class WinScreen : public Screen {
  public:

    WinScreen(GameState gs);

    bool update(const Input&, Audio&, unsigned int) override;
    void draw(Graphics&) const override;

    Screen* next_screen() const override;

  private:

    static const std::vector<std::string> kStoryText;

    GameState gs_;
    Sprite rocket_;
    SpriteMap fire_;
    Dialog dialog_;
    int timer_;
    size_t story_index_;
};
