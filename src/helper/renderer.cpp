

#include "ini.hpp"

#include "helper/renderer.h"

#include "utils.h"

Renderer::Renderer(const std::string &title, const std::string &config_path) {
  ini::IniFile data;
  try {
    data.load(config_path);
  } catch (const std::exception &e) {
    fmt::panic("Failed to load config file: %s", e.what());
  }

  auto &window = data["window"];
  m_width = window["width"].as<int>();
  m_height = window["height"].as<int>();
  m_fullscreen = window["fullscreen"].as<bool>();

  auto &engine = data["engine"];
  m_fps = engine["fps"].as<int>();
  m_ups = engine["ups"].as<int>();

  auto &animation = data["animation"];
  m_update_interval = animation["update_interval"].as<int>();

  m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, m_width, m_height,
                              SDL_WINDOW_SHOWN);
  if (m_window == nullptr) {
    fmt::panic("Failed to create window: %s", SDL_GetError());
  }

  m_surface = SDL_GetWindowSurface(m_window);
  if (m_surface == nullptr) {
    fmt::panic("Failed to get window surface: %s", SDL_GetError());
  }
}
