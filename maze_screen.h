#pragma once

#include "screen.h"
#include "text.h"

#include "camera.h"
#include "dialog.h"
#include "maze_player.h"
#include "warehouse.h"

class MazeScreen : public Screen {
  public:

    MazeScreen();

    bool update(const Input&, Audio&, unsigned int) override;
    void draw(Graphics&) const override;

    Screen* next_screen() const override;

  private:

    Text text_;
    Warehouse warehouse_;
    MazePlayer player_;
    Camera camera_;
    Dialog dialog_;
};
