

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

  m_sprites = m_assets->get_surface();
  SDL_Rect src_bg = m_assets->m_bg;

  double scale_x = static_cast<double>(m_width) / src_bg.w;
  double scale_y = static_cast<double>(m_height) / src_bg.h;
  m_scale = std::min(scale_x, scale_y);
  m_rect_mode = RectMode::CORNER;
}

Renderer::~Renderer() {
  SDL_DestroyWindow(m_window);
  m_window = nullptr;
  m_surface = nullptr;
}

std::shared_ptr<Assets> Renderer::get_assets(void) const { return m_assets; }

void Renderer::add_showable(std::shared_ptr<Showable> showable) {
  m_showables.push_back(showable);
}

void Renderer::remove_showable(std::shared_ptr<Showable> showable) {
  m_showables.erase(
      std::remove(m_showables.begin(), m_showables.end(), showable),
      m_showables.end());
}

void Renderer::flip(void) { SDL_UpdateWindowSurface(m_window); }

void Renderer::clear(void) {
  SDL_FillRect(m_surface, nullptr, SDL_MapRGB(m_surface->format, 0, 0, 0));
}

void Renderer::blit(SDL_Rect src, int x, int y) {

  int w = static_cast<int>(src.w * m_scale);
  int h = static_cast<int>(src.h * m_scale);

  switch (m_rect_mode) {
  case RectMode::CORNER:
    break;
  case RectMode::CENTER:
    x -= w / 2;
    y -= h / 2;
    break;
  }

  SDL_Rect dest = {x, y, w, h};
  SDL_SetColorKey(m_sprites, false, 0);
  SDL_BlitScaled(m_sprites, &src, m_surface, &dest);
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

void Renderer::rect_mode(RectMode mode) { m_rect_mode = mode; }
