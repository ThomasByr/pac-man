
#include <SDL.h>

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "helper/assets.h"
#include "json.hpp"

#include "utils.h"

SDL_Rect from_json(const nlohmann::json &j) {
  SDL_Rect rect;
  try {
    rect.x = j["x"];
    rect.y = j["y"];
    rect.w = j["w"];
    rect.h = j["h"];
  } catch (const std::exception &e) {
    fmt::panic("Failed to parse json: %s", e.what());
  }

  return rect;
}

Assets::Assets(const std::string &path) : m_surface{nullptr} {
  using make_pair = std::pair<char, SDL_Rect>;

  const std::string filename = path.substr(0, path.find_last_of('.'));
  const std::ifstream file(fmt::format("%s.json", filename.c_str()));

  std::stringstream buffer{};
  buffer << file.rdbuf();
  auto data = nlohmann::json::parse(buffer.str());

  // load the surface
  m_surface = SDL_LoadBMP(path.c_str());

  if (m_surface == nullptr) {
    fmt::panic("Failed to load surface: %s", SDL_GetError());
  }

  // set the rectangles for the sprites
  // todo

  auto _data_bg = data["bg"];
  m_bg = from_json(_data_bg);

  auto _daga_bg_menu = data["bg_menu"];
  m_bg_menu = from_json(_daga_bg_menu);

  auto _data_m_lives = data["m_lives"];
  m_lives = from_json(_data_m_lives);

  // set dots
  auto _data_dot = data["m_dot"];
  m_dot = from_json(_data_dot);
  auto _data_power_dot = data["m_power_dot"];
  m_power_dot = from_json(_data_power_dot);

  // set the alpha-numerical characters
  auto _data_alpha_numerical = data["alpha-numerical-default"];
  int x = -1, y = -1, w = -1, h = -1;
  x = _data_alpha_numerical["x"];
  y = _data_alpha_numerical["y"];
  w = _data_alpha_numerical["w"];
  h = _data_alpha_numerical["h"];
  m_alpha_numerical_default = {x, y, w, h};

  _data_alpha_numerical = data["alpha-numerical-0"];
  x = _data_alpha_numerical["x"];
  y = _data_alpha_numerical["y"];
  w = _data_alpha_numerical["w"];
  h = _data_alpha_numerical["h"];
  // set "[0-9]-/!,"
  for (int i = 0; i < 10; i++) { /* 0-9 */
    m_alpha_numerical.insert(make_pair{'0' + i, {x + i * (w + 1), y, w, h}});
  }
  m_alpha_numerical.insert(make_pair{'-', {x + 10 * (w + 1), y, w, h}});
  m_alpha_numerical.insert(make_pair{'/', {x + 11 * (w + 1), y, w, h}});
  m_alpha_numerical.insert(make_pair{'!', {x + 12 * (w + 1), y, w, h}});
  m_alpha_numerical.insert(make_pair{',', {x + 13 * (w + 1), y, w, h}});
  // set " [A-O]"
  y += h + 1;
  m_alpha_numerical.insert(make_pair{' ', {x, y, w, h}});
  for (int i = 0; i < 15; i++) { /* A-O */
    m_alpha_numerical.insert(
      make_pair{'A' + i, {x + (i + 1) * (w + 1), y, w, h}});
  }
  // set "[P-Z].> c""
  y += h + 1;
  for (int i = 0; i < 11; i++) { /* P-Z */
    m_alpha_numerical.insert(make_pair{'P' + i, {x + i * (w + 1), y, w, h}});
  }
  m_alpha_numerical.insert(make_pair{'.', {x + 11 * (w + 1), y, w, h}});
  m_alpha_numerical.insert(make_pair{'>', {x + 12 * (w + 1), y, w, h}});
  m_alpha_numerical.insert(make_pair{'c', {x + 14 * (w + 1), y, w, h}});
  m_alpha_numerical.insert(make_pair{'"', {x + 15 * (w + 1), y, w, h}});

  // set m_pacman_up for all frames
  auto _data_pacman_up = data["m_pacman_up"];
  for (size_t i = 0; i < _data_pacman_up.size(); i++) {
    m_pacman_up.push_back(from_json(_data_pacman_up[i]));
  }
  for (size_t i = _data_pacman_up.size() - 1; i > 0; i--) {
    m_pacman_up.push_back(from_json(_data_pacman_up[i]));
  }
  // set m_pacman_down for all frames
  auto _data_pacman_down = data["m_pacman_down"];
  for (size_t i = 0; i < _data_pacman_down.size(); i++) {
    m_pacman_down.push_back(from_json(_data_pacman_down[i]));
  }
  for (size_t i = _data_pacman_down.size() - 1; i > 0; i--) {
    m_pacman_down.push_back(from_json(_data_pacman_down[i]));
  }
  // set m_pacman_left for all frames
  auto _data_pacman_left = data["m_pacman_left"];
  for (size_t i = 0; i < _data_pacman_left.size(); i++) {
    m_pacman_left.push_back(from_json(_data_pacman_left[i]));
  }
  for (size_t i = _data_pacman_left.size() - 1; i > 0; i--) {
    m_pacman_left.push_back(from_json(_data_pacman_left[i]));
  }
  // set m_pacman_right for all frames
  auto _data_pacman_right = data["m_pacman_right"];
  for (size_t i = 0; i < _data_pacman_right.size(); i++) {
    m_pacman_right.push_back(from_json(_data_pacman_right[i]));
  }
  for (size_t i = _data_pacman_right.size() - 1; i > 0; i--) {
    m_pacman_right.push_back(from_json(_data_pacman_right[i]));
  }

  // set m_redghost_xx for all frames
  auto _data_redghost_up = data["m_redghost_up"];
  for (size_t i = 0; i < _data_redghost_up.size(); i++) {
    m_redghost_up.push_back(from_json(_data_redghost_up[i]));
  }

  auto _data_redghost_left = data["m_redghost_left"];
  for (size_t i = 0; i < _data_redghost_left.size(); i++) {
    m_redghost_left.push_back(from_json(_data_redghost_left[i]));
  }

  auto _data_redghost_right = data["m_redghost_right"];
  for (size_t i = 0; i < _data_redghost_right.size(); i++) {
    m_redghost_right.push_back(from_json(_data_redghost_right[i]));
  }

  auto _data_redghost_down = data["m_redghost_down"];
  for (size_t i = 0; i < _data_redghost_down.size(); i++) {
    m_redghost_down.push_back(from_json(_data_redghost_down[i]));
  }

  // set m_cyanghost_xx for all frames
  auto _data_cyanghost_up = data["m_cyanghost_up"];
  for (size_t i = 0; i < _data_cyanghost_up.size(); i++) {
    m_cyanghost_up.push_back(from_json(_data_cyanghost_up[i]));
  }

  auto _data_cyanghost_left = data["m_cyanghost_left"];
  for (size_t i = 0; i < _data_cyanghost_left.size(); i++) {
    m_cyanghost_left.push_back(from_json(_data_cyanghost_left[i]));
  }

  auto _data_cyanghost_right = data["m_cyanghost_right"];
  for (size_t i = 0; i < _data_cyanghost_right.size(); i++) {
    m_cyanghost_right.push_back(from_json(_data_cyanghost_right[i]));
  }

  auto _data_cyanghost_down = data["m_cyanghost_down"];
  for (size_t i = 0; i < _data_cyanghost_down.size(); i++) {
    m_cyanghost_down.push_back(from_json(_data_cyanghost_down[i]));
  }

  // set m_pinkghost_xx for all frames
  auto _data_pinkghost_up = data["m_pinkghost_up"];
  for (size_t i = 0; i < _data_pinkghost_up.size(); i++) {
    m_pinkghost_up.push_back(from_json(_data_pinkghost_up[i]));
  }

  auto _data_pinkghost_left = data["m_pinkghost_left"];
  for (size_t i = 0; i < _data_pinkghost_left.size(); i++) {
    m_pinkghost_left.push_back(from_json(_data_pinkghost_left[i]));
  }

  auto _data_pinkghost_right = data["m_pinkghost_right"];
  for (size_t i = 0; i < _data_pinkghost_right.size(); i++) {
    m_pinkghost_right.push_back(from_json(_data_pinkghost_right[i]));
  }

  auto _data_pinkghost_down = data["m_pinkghost_down"];
  for (size_t i = 0; i < _data_pinkghost_down.size(); i++) {
    m_pinkghost_down.push_back(from_json(_data_pinkghost_down[i]));
  }

  // set m_orangeghost_xx for all frames
  auto _data_orangeghost_up = data["m_orangeghost_up"];
  for (size_t i = 0; i < _data_orangeghost_up.size(); i++) {
    m_orangeghost_up.push_back(from_json(_data_orangeghost_up[i]));
  }

  auto _data_orangeghost_left = data["m_orangeghost_left"];
  for (size_t i = 0; i < _data_orangeghost_left.size(); i++) {
    m_orangeghost_left.push_back(from_json(_data_orangeghost_left[i]));
  }

  auto _data_orangeghost_right = data["m_orangeghost_right"];
  for (size_t i = 0; i < _data_orangeghost_right.size(); i++) {
    m_orangeghost_right.push_back(from_json(_data_orangeghost_right[i]));
  }

  auto _data_orangeghost_down = data["m_orangeghost_down"];
  for (size_t i = 0; i < _data_orangeghost_down.size(); i++) {
    m_orangeghost_down.push_back(from_json(_data_orangeghost_down[i]));
  }

  auto _data_eyes_up = data["m_eyes_up"];
  m_eyes_up = from_json(_data_eyes_up);
  auto _data_eyes_left = data["m_eyes_left"];
  m_eyes_left = from_json(_data_eyes_left);
  auto _data_eyes_right = data["m_eyes_right"];
  m_eyes_right = from_json(_data_eyes_right);
  auto _data_eyes_down = data["m_eyes_down"];
  m_eyes_down = from_json(_data_eyes_down);

  auto _data_weak_ghost = data["m_weak_ghost"];
  for (size_t i = 0; i < _data_weak_ghost.size(); i++) {
    m_weak_ghost.push_back(from_json(_data_weak_ghost[i]));
  }
  auto _data_weak_ghost_ending = data["m_weak_ghost_ending"];
  for (size_t i = 0; i < _data_weak_ghost_ending.size(); i++) {
    m_weak_ghost_ending.push_back(from_json(_data_weak_ghost_ending[i]));
  }
}

Assets::~Assets() { SDL_FreeSurface(m_surface); }

SDL_Rect Assets::get_sprite_alpha_numerical(char c) const {
  auto it = m_alpha_numerical.find(c);
  if (it == m_alpha_numerical.end()) {
    fmt::debug("Failed to find sprite for character: %c", c);
    return m_alpha_numerical_default;
  }

  return it->second;
}

SDL_Surface *Assets::get_surface() const { return m_surface; }

SDL_Rect Assets::get_sprite_dot() const { return m_dot; }
SDL_Rect Assets::get_sprite_power_dot() const { return m_power_dot; }

SDL_Rect Assets::get_sprite_pacman(const Direction &dir, int fc) const {
  switch (dir) {
  case Direction::UP: return m_pacman_up.at(fc % m_pacman_up.size());
  case Direction::DOWN: return m_pacman_down.at(fc % m_pacman_down.size());
  case Direction::LEFT: return m_pacman_left.at(fc % m_pacman_left.size());
  case Direction::RIGHT: return m_pacman_right.at(fc % m_pacman_right.size());
  default: // fallback for Direction::NONE
    return m_pacman_up.at(0);
  }
}

SDL_Rect Assets::get_sprite_ghost_red(const Direction &dir, int fc) const {
  switch (dir) {
  case Direction::UP: return m_redghost_up.at(fc % m_redghost_up.size());
  case Direction::DOWN: return m_redghost_down.at(fc % m_redghost_down.size());
  case Direction::LEFT: return m_redghost_left.at(fc % m_redghost_left.size());
  case Direction::RIGHT:
    return m_redghost_right.at(fc % m_redghost_right.size());
  default: // fallback for Direction::NONE
    return m_redghost_up.at(0);
  }
}

SDL_Rect Assets::get_sprite_ghost_blue(const Direction &dir, int fc) const {
  switch (dir) {
  case Direction::UP: return m_cyanghost_up.at(fc % m_cyanghost_up.size());
  case Direction::DOWN:
    return m_cyanghost_down.at(fc % m_cyanghost_down.size());
  case Direction::LEFT:
    return m_cyanghost_left.at(fc % m_cyanghost_left.size());
  case Direction::RIGHT:
    return m_cyanghost_right.at(fc % m_cyanghost_right.size());
  default: // fallback for Direction::NONE
    return m_cyanghost_up.at(0);
  }
}

SDL_Rect Assets::get_sprite_ghost_pink(const Direction &dir, int fc) const {
  switch (dir) {
  case Direction::UP: return m_pinkghost_up.at(fc % m_pinkghost_up.size());
  case Direction::DOWN:
    return m_pinkghost_down.at(fc % m_pinkghost_down.size());
  case Direction::LEFT:
    return m_pinkghost_left.at(fc % m_pinkghost_left.size());
  case Direction::RIGHT:
    return m_pinkghost_right.at(fc % m_pinkghost_right.size());
  default: // fallback for Direction::NONE
    return m_pinkghost_up.at(0);
  }
}

SDL_Rect Assets::get_sprite_ghost_orange(const Direction &dir, int fc) const {
  switch (dir) {
  case Direction::UP: return m_orangeghost_up.at(fc % m_orangeghost_up.size());
  case Direction::DOWN:
    return m_orangeghost_down.at(fc % m_orangeghost_down.size());
  case Direction::LEFT:
    return m_orangeghost_left.at(fc % m_orangeghost_left.size());
  case Direction::RIGHT:
    return m_orangeghost_right.at(fc % m_orangeghost_right.size());
  default: // fallback for Direction::NONE
    return m_orangeghost_up.at(0);
  }
}

SDL_Rect Assets::get_sprite_ghost_weak(int fc, bool ending) const {
  if (ending) {
    return m_weak_ghost_ending.at(fc % m_weak_ghost_ending.size());
  }
  return m_weak_ghost.at(fc % m_weak_ghost.size());
}

SDL_Rect Assets::get_sprite_ghost_eyes(const Direction &dir) const {
  switch (dir) {
  case Direction::UP: return m_eyes_up;
  case Direction::DOWN: return m_eyes_down;
  case Direction::LEFT: return m_eyes_left;
  case Direction::RIGHT: return m_eyes_right;
  default: // fallback for Direction::NONE
    return m_eyes_up;
  }
}
