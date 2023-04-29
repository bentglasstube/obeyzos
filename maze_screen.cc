#include "maze_screen.h"

#include "util.h"

MazeScreen::MazeScreen() : text_("text.png"), warehouse_(Util::random_seed()) {}

bool MazeScreen::update(const Input& input, Audio&, unsigned int elapsed) {
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
        break;

      case Warehouse::Tile::Shelf:
        // TODO talk about boxes
        break;

      case Warehouse::Tile::WallFace:
        // TODO check sprite and talk about anti-union posters
        break;

      default: // Other tiles do nothing interesting
        break;
    }
  }

  // TODO handle workers
  // TODO handle busters

  player_.update(warehouse_, elapsed);

  const int px = std::floor(player_.x() / Config::kTileSize);
  const int py = std::floor(player_.y() / Config::kTileSize);
  warehouse_.calculate_visibility(px, py);

  return true;
}

void MazeScreen::draw(Graphics& graphics) const {
  warehouse_.draw(graphics, 0, 0);
  player_.draw(graphics, 0, 0);
}

Screen* MazeScreen::next_screen() const {
  return nullptr;
}
