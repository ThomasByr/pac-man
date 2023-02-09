
#include <SDL2/SDL.h>

#include <iostream>
#include <sstream>
#include <string>

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
    std::stringstream ss;
    ss << "Failed to parse json: " << e.what() << std::endl;
    throw std::runtime_error(ss.str());
  }

  return rect;
}

Assets::Assets(const std::string &path) {
  const std::string filename = path.substr(0, path.find_last_of('.'));
  auto data = nlohmann::json::parse(fmt::format("%s.json", filename.c_str()));

  // load the surface
  m_surface = SDL_LoadBMP(path.c_str());

  if (m_surface == nullptr) {
    std::stringstream ss;
    ss << "Failed to load assets: " << SDL_GetError() << std::endl;
    throw std::runtime_error(ss.str());
  }

  // set the rectangles for the sprites
  // todo

  // set m_pacman_up for all frames
  for (size_t i = 0; i < data["pacman_up"].size(); i++) {
    m_pacman_up.push_back(from_json(data["pacman_up"][i]));
  }
}

Assets::~Assets() { SDL_FreeSurface(m_surface); }
