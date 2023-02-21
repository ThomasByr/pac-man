

#include "ini.hpp"

#include "helper/assets.h"
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

  auto &ressource = data["ressource"];
  std::string base_path = ressource["path"].as<std::string>();
  std::string image_path = base_path + ressource["sprites"].as<std::string>();
  m_assets = std::make_shared<Assets>(image_path);

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

Renderer::~Renderer() {
  SDL_DestroyWindow(m_window);
  m_window = nullptr;
  m_surface = nullptr;
}

void Renderer::add_showable(std::shared_ptr<Showable> showable) {
  m_showables.push_back(showable);
}

void Renderer::remove_showable(std::shared_ptr<Showable> showable) {
  m_showables.erase(
      std::remove(m_showables.begin(), m_showables.end(), showable),
      m_showables.end());
}

void Renderer::flip(void) {
  // surface of the spritesheet
  SDL_Surface *surface = m_assets->get_surface();
  SDL_Rect src_bg = m_assets->m_bg;

  double scale_x = static_cast<double>(m_width) / src_bg.w;
  double scale_y = static_cast<double>(m_height) / src_bg.h;
  double scale = std::min(scale_x, scale_y);

  SDL_Rect dest_bg = {0, 0, static_cast<int>(src_bg.w * scale),
                      static_cast<int>(src_bg.h * scale)};

  SDL_SetColorKey(surface, false, 0);
  SDL_BlitScaled(surface, &src_bg, m_surface, &dest_bg);

  for (auto &showable : m_showables) { showable->show(m_surface); }

  SDL_UpdateWindowSurface(m_window);
}

void Renderer::text(const std::string &text, int x, int y) {
  // as we do not use sdl_ttf, we have to render the text ourselves
  // we can use sprites for that
  // we have to render each character separately (each is 7)

  (void)text;
  (void)x;
  (void)y;

  // todo: implement
  
  // for (auto &c : text) {
  //   SDL_Rect sprite = m_assets->get_sprite_alpha_numerical(c);
  //   SDL_Rect dest = {x, y, sprite.w, sprite.h};
    
  //   x += 7;
  // }
}
