#pragma once

#include <sstream>
#include <string>

#include "util.h"

struct GameState {
  const unsigned long seed = Util::random_seed();
  int workers = 1;
  int time = 9 * 60000; // 9 am

  void add_time(unsigned int elapsed) { time += elapsed; }
  bool day_over() const { return time > 17 * 60000; }

  std::string clock() const {
    const int hour = (time / 60000);
    const int minute = (time / 1000) % 60;

    std::stringstream ss;
    ss << (hour > 12 ? hour - 12 : hour) << ":";
    ss << (minute < 10 ? "0" : "") << minute;
    ss << " " << (hour > 12 ? "pm" : "am");
    return ss.str();
  }
};
