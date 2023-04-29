#include <iostream>

#include "state.h"

#include "utils.h"

std::ostream &operator<<(std::ostream &os, const GameState &state) {
  os << to_string(state);
  return os;
}

std::string to_string(const GameState &state) {
  std::string str;
  switch (state) {
  case GameState::MENU: str = "MENU"; break;
  case GameState::GAME: str = "GAME"; break;
  case GameState::PAUSE: str = "PAUSE"; break;
  case GameState::GAME_OVER: str = "GAME_OVER"; break;
  case GameState::EXIT: str = "EXIT"; break;
  default: fmt::panic("Invalid game state");
  }
  return str;
}

std::ostream &operator<<(std::ostream &os, const PcmnState &state) {
  os << to_string(state);
  return os;
}

std::string to_string(const PcmnState &state) {
  std::string str;
  switch (state) {
  case PcmnState::ALIVE: str = "ALIVE"; break;
  case PcmnState::DEAD: str = "DEAD"; break;
  case PcmnState::POWERED: str = "POWERED"; break;
  default: fmt::panic("Invalid pacman state");
  }
  return str;
}
