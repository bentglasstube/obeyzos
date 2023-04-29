#include "dialog.h"

Dialog::Dialog() :
  text_("text.png"),
  box_("box.png", 8, 8),
  message_(""), timer_(0), index_(0) {}

void Dialog::set_message(const std::string& message) {
  message_ = message;
  index_ = 0;
}

void Dialog::update(unsigned int elapsed) {
  if (!done()) {
    timer_ += elapsed;
    if (timer_ > kRate) {
      ++index_;
    }
  }
}

void Dialog::draw(Graphics& graphics) const {
  box_.draw(graphics, 2 * kPadding, kPadding, graphics.width() - 4 * kPadding, kHeight);
  text_.draw(graphics, message_.substr(0, index_), 80, 48);
}
