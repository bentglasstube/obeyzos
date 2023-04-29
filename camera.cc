#include "camera.h"

#include <algorithm>

#include "config.h"

Camera::Camera() : xo_(0), yo_(0) {}

void Camera::update(const Rect& f, unsigned int elapsed) {
  const double max = kMaxSpeed * elapsed;

  const double fx = f.left;
  const double fy = f.top;

  const Rect target = focus();

  // TODO check for wrapping

  if (fx < target.left) {
    xo_ -= std::min(target.left - fx, max);
  } else if (fx > target.right) {
    xo_ += std::min(fx - target.right, max);
  }

  if (fy < target.top) {
    yo_ -= std::min(target.top - fy, max);
  } else if (fy > target.bottom) {
    yo_ += std::min(fy - target.bottom, max);
  }

  xo_ = std::clamp(xo_, fx - kConfig.graphics.width, fx);
  yo_ = std::clamp(yo_, fy - kConfig.graphics.height, fy);
}

void Camera::snap(const Rect& focus) {
  xo_ = focus.left - kConfig.graphics.width / 2;
  yo_ = focus.top - kConfig.graphics.height / 2;
}

Rect Camera::focus() const {
  return Rect(
      xo_ + kConfig.graphics.width * (0.5 - kHorzFocus),
      yo_ + kConfig.graphics.height * (0.5 - kVertFocus),
      xo_ + kConfig.graphics.width * (0.5 + kHorzFocus),
      yo_ + kConfig.graphics.height * (0.5 + kVertFocus));
}
