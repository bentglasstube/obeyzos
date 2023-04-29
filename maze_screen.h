#pragma once

#include <random>
#include <vector>

#include "screen.h"
#include "text.h"

#include "camera.h"
#include "dialog.h"
#include "maze_player.h"
#include "warehouse.h"
#include "worker.h"

class MazeScreen : public Screen {
  public:

    MazeScreen();

    bool update(const Input&, Audio&, unsigned int) override;
    void draw(Graphics&) const override;

    Screen* next_screen() const override;

  private:

    std::mt19937 rng_;

#ifndef NDEBUG
    bool cheater_mode_ = false;
#endif

    Text text_;
    Warehouse warehouse_;
    MazePlayer player_;
    Camera camera_;
    Dialog dialog_;
    std::vector<Worker> workers_;
};
