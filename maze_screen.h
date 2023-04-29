#pragma once

#include <random>
#include <vector>

#include "screen.h"
#include "text.h"

#include "camera.h"
#include "dialog.h"
#include "game_state.h"
#include "maze_player.h"
#include "warehouse.h"
#include "worker.h"

class MazeScreen : public Screen {
  public:

    MazeScreen(GameState gs);

    bool update(const Input&, Audio&, unsigned int) override;
    void draw(Graphics&) const override;

    Screen* next_screen() const override;

  private:

    GameState gs_;
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

    int union_workers() const;
};
