#include "config.h"

Config::Config() : Game::Config() {
  graphics.title = "Obeyzos";
  graphics.width = 512;
  graphics.height = 480;
  graphics.intscale = 2;
  graphics.fullscreen = false;
}
