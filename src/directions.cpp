#include <iostream>

#include "directions.h"

std::ostream &operator<<(std::ostream &os, const Direction &dir) {
  os << to_string(dir);
  return os;
}

std::string to_string(const Direction &dir) {
  std::string str;
  switch (dir) {
  case Direction::UP:
    str = "UP";
    break;
  case Direction::DOWN:
    str = "DOWN";
    break;
  case Direction::LEFT:
    str = "LEFT";
    break;
  case Direction::RIGHT:
    str = "RIGHT";
    break;
  default:
    throw std::runtime_error("Invalid direction");
  }
  return str;
}

Direction opposite(const Direction &dir) {
  Direction opposite;
  switch (dir) {
  case Direction::UP:
    opposite = Direction::DOWN;
    break;
  case Direction::DOWN:
    opposite = Direction::UP;
    break;
  case Direction::LEFT:
    opposite = Direction::RIGHT;
    break;
  case Direction::RIGHT:
    opposite = Direction::LEFT;
    break;
  default:
    throw std::runtime_error("Invalid direction");
  }
  return opposite;
}

Direction turn_left(const Direction &dir) {
  Direction left;
  switch (dir) {
  case Direction::UP:
    left = Direction::LEFT;
    break;
  case Direction::DOWN:
    left = Direction::RIGHT;
    break;
  case Direction::LEFT:
    left = Direction::DOWN;
    break;
  case Direction::RIGHT:
    left = Direction::UP;
    break;
  default:
    throw std::runtime_error("Invalid direction");
  }
  return left;
}

Direction turn_right(const Direction &dir) {
  /* return turn_left(opposite(dir)); */
  Direction right;
  switch (dir) {
  case Direction::UP:
    right = Direction::RIGHT;
    break;
  case Direction::DOWN:
    right = Direction::LEFT;
    break;
  case Direction::LEFT:
    right = Direction::UP;
    break;
  case Direction::RIGHT:
    right = Direction::DOWN;
    break;
  default:
    throw std::runtime_error("Invalid direction");
  }
  return right;
}
