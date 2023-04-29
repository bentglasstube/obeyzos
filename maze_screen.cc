#include "maze_screen.h"

#include "util.h"

MazeScreen::MazeScreen() :
  rng_(Util::random_seed()),
  text_("text.png"),
  warehouse_(rng_()),
  camera_(player_.draw_box())
{
  // add workers
  for (int i = 0; i < 10; ++i) {
    auto p = warehouse_.random_open_cell(rng_);
    workers_.emplace_back(
        rng_(),
        p.first * Config::kTileSize + Config::kTileSize / 2,
        p.second * Config::kTileSize + Config::kTileSize / 2);
  }
}

bool MazeScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  if (dialog_) {
    dialog_.update(elapsed);
    if (input.key_pressed(Input::Button::A)) {
      if (dialog_.done()) {
        dialog_.dismiss();
      } else {
        dialog_.finish();
      }
    }

    return true;
  }

#ifndef NDEBUG
  if (input.key_pressed(Input::Button::Select)) cheater_mode_ = !cheater_mode_;
#endif

  if (input.key_held(Input::Button::Left)) {
    player_.move(Direction::West);
  } else if (input.key_held(Input::Button::Right)) {
    player_.move(Direction::East);
  } else if (input.key_held(Input::Button::Up)) {
    player_.move(Direction::North);
  } else if (input.key_held(Input::Button::Down)) {
    player_.move(Direction::South);
  } else {
    player_.stop();
  }

  if (input.key_pressed(Input::Button::A)) {
    const Warehouse::Cell c = player_.interact(warehouse_);

    switch (c.tile) {
      case Warehouse::Tile::Elevator:
        // TODO go up
        return false;

      case Warehouse::Tile::Shelf:
        dialog_.set_message("Look at all this junk.");
        break;

      case Warehouse::Tile::WallFace:
        // Only sprites above 8 have "posters"
        if (c.sprite > 8) dialog_.set_message("More anti-union propaganda.");
        break;

      default: // Other tiles do nothing interesting
        break;
    }
  }

  // TODO handle busters

  for (auto& worker : workers_) {
    worker.update(warehouse_, elapsed);
  }

  player_.update(warehouse_, elapsed);
  camera_.update(player_.draw_box(), elapsed);

  const int px = std::floor(player_.x() / Config::kTileSize);
  const int py = std::floor(player_.y() / Config::kTileSize);
  warehouse_.calculate_visibility(px, py);

  return true;
}

void MazeScreen::draw(Graphics& graphics) const {
  const int xo = std::floor(camera_.xoffset());
  const int yo = std::floor(camera_.yoffset());

#ifndef NDEBUG
  warehouse_.draw(graphics, xo, yo, cheater_mode_);
#else
  warehouse_.draw(graphics, xo, yo);
#endif
  player_.draw(graphics, xo, yo);
  for (auto worker : workers_) {
    if (warehouse_.box_visible(worker.collision_box())) {
      worker.draw(graphics, xo, yo);
    }
  }
  if (dialog_) dialog_.draw(graphics);
}

Screen* MazeScreen::next_screen() const {
  return nullptr;
}
