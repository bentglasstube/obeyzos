#include "maze_screen.h"

#include "puzzle_screen.h"
#include "util.h"

MazeScreen::MazeScreen(GameState gs) :
  gs_(gs),
  rng_(Util::random_seed()),
  dialog_index_(0),
  text_("text.png"),
  warehouse_(rng_()),
  camera_(player_.draw_box())
{
  for (int i = 0; i < kNumWorkers; ++i) {
    workers_.emplace_back(rng_(), warehouse_.random_open_cell(rng_));
  }
  for (int i = 0; i < kNumBusters; ++i) {
    busters_.emplace_back(rng_(), warehouse_.random_open_cell(rng_));
  }
}

bool MazeScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  gs_.add_time(elapsed);

  const int px = std::floor(player_.x() / Config::kTileSize);
  const int py = std::floor(player_.y() / Config::kTileSize);
  warehouse_.calculate_visibility(px, py);

  if (dialog_) {
    dialog_.update(input, elapsed);
    return true;
  } else {
    if (dialog_index_ < kIntroText.size()) {
      dialog_.set_message(kIntroText[dialog_index_]);
      ++dialog_index_;
      return true;
    }
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
    bool worker_found = false;
    for (auto& worker : workers_) {
      if (worker.draw_box().intersect(player_.target())) {
        if (worker.is_unionized()) {
          dialog_.set_message("I'm already in the union.");
        } else {
          dialog_.set_message("Yes, I'd love to join the union.");
          worker.unionize();
        }
        worker_found = true;
      }
    }

    if (!worker_found) {
      const Warehouse::Cell c = player_.interact(warehouse_);

      switch (c.tile) {
        case Warehouse::Tile::Elevator:
          // TODO go up
          gs_.workers += union_workers();
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
  }

  for (auto& buster : busters_) {
    if (warehouse_.box_visible(buster.collision_box())) {
      buster.chase(player_);
    } else {
      buster.stop_chasing();
    }
    buster.update(warehouse_, elapsed);

    if (buster.collision_box().intersect(player_.collision_box())) {
      // TODO fade out/in
      dialog_.set_message("Hey kid, time for workplace training!");
      gs_.add_time(60000);
      player_.reset_position();
      camera_.snap(player_.draw_box());

      std::uniform_int_distribution<int> flip(0, 1);
      for (auto& worker : workers_) {
        if (flip(rng_) == 1) worker.leave_union();
        worker.set_tile_position(warehouse_.random_open_cell(rng_));
      }
      for (auto& buster : busters_) {
        buster.set_tile_position(warehouse_.random_open_cell(rng_));
      }
    }
  }

  for (auto& worker : workers_) {
    worker.update(warehouse_, elapsed);
  }

  player_.update(warehouse_, elapsed);
  camera_.update(player_.draw_box(), elapsed);

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

  for (auto buster : busters_) {
    buster.draw(graphics, xo, yo);
  }

  if (dialog_) dialog_.draw(graphics);

  text_.draw(graphics, "Workers: " + std::to_string(gs_.workers + union_workers()), 0, 0);
  text_.draw(graphics, gs_.clock(), graphics.width(), 0, Text::Alignment::Right);
}

Screen* MazeScreen::next_screen() const {
  return new PuzzleScreen(gs_);
}

int MazeScreen::union_workers() const {
  int sum = 0;
  for (const auto& worker : workers_) {
    if (worker.is_unionized()) ++sum;
  }
  return sum;
}

const std::vector<std::string> MazeScreen::kIntroText = {
  //-----------------------------------------//
  "Just another day at work in the NILE\n"
  "factory warehouse.  Normally I'm a\n"
  "delivery driver, but I decided to come\n"
  "to the warehouse today.",

  "The warehouse workers have it even worse\n"
  "than us drivers, so I am hoping to talk\n"
  "them into joining our union.",

  "If we can get enough people to join, we\n"
  "can take on the CEO, GEOFF BOZOS, and\n"
  "demand better working conditions.",

  "It won't be easy, with all the managers\n"
  "running around \"reeducating\" people for\n"
  "trying to organize.  At least they are\n"
  "easy to spot in their yellow jumpsuits.",
};
