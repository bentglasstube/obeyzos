#pragma once

#include <string>

#include "box.h"
#include "input.h"
#include "text.h"

#include "config.h"

class Dialog {
  public:

    Dialog();

    void set_message(const std::string& message);
    void update(const Input& input, unsigned int elapsed);
    void draw(Graphics& graphics, bool include_box=true) const;
    bool done() const { return index_ >= message_.length(); }
    void finish() { index_ = message_.length(); }
    void dismiss() { message_ = ""; }

    operator bool() const { return message_.length() > 0; }

  private:

    static constexpr int kRate = 50;
    static constexpr int kPadding = 2 * Config::kTileSize;
    static constexpr int kLines = 4;
    static constexpr int kHeight = (2 + kLines) * Config::kTileSize;

    Text text_;
    Box box_;
    std::string message_;
    int timer_;
    size_t index_;
};
