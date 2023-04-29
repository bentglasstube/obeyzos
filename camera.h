#pragma once

#include "rect.h"

class Camera {
  public:

    Camera();

    void update(const Rect& focus, unsigned int elapsed);
    void snap(const Rect& focus);

    double xoffset() const { return xo_; }
    double yoffset() const { return yo_; }

    Rect focus() const;

  private:

    static constexpr double kHorzFocus = 0.167;
    static constexpr double kVertFocus = 0.25;
    static constexpr double kMaxSpeed = 0.4;

    double xo_, yo_;
};
