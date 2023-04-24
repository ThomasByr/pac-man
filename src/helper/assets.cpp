
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
  auto _data_pacman_up = data["pacman_up"];
  for (size_t i = 0; i < _data_pacman_up.size(); i++) {
    m_pacman_up.push_back(from_json(_data_pacman_up[i]));
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
  case Direction::UP: return m_pacman_up[fc % m_pacman_up.size()];
  case Direction::DOWN: return m_pacman_down[fc % m_pacman_down.size()];
  case Direction::LEFT: return m_pacman_left[fc % m_pacman_left.size()];
  case Direction::RIGHT: return m_pacman_right[fc % m_pacman_right.size()];
  default: // fallback for Direction::NONE
    return m_pacman_up[0];
  }
}
