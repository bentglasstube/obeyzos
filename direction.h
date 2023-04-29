#pragma once

#include <random>

class Direction {
  public:
    enum Value : uint8_t { North, East, South, West };

    constexpr Direction() : value_(North) {}
    constexpr Direction(Value v) : value_(v) {}
    operator Value() const { return value_; }
    explicit operator bool() = delete;

    Direction reverse() const {
      switch (value_) {
        case North: return South;
        case South: return North;
        case East:  return West;
        case West:  return East;
        default:    return North;
      }
    }

    template <class Generator> static Direction random(Generator& g) {
      return (Value)std::uniform_int_distribution<uint8_t>(0, 3)(g);
    }

    int dx() const {
      switch(value_) {
        case East:  return 1;
        case West:  return -1;
        default:    return 0;
      }
    }

    int dy() const {
      switch(value_) {
        case North: return -1;
        case South: return 1;
        default:    return 0;
      }
    }

  private:
    Value value_;
};
