
#include <thread>

#include "ini.hpp"

#include "helper/assets.h"
#include "helper/renderer.h"

#include "utils.h"

Renderer::Renderer(const std::string &title, const std::string &config_path)
  : m_window{nullptr}, m_surface{nullptr}, m_sprites{nullptr}, m_scale{1},
    size{0}, m_config_stack{},
    m_rect_mode{RectMode::CORNER}, m_trans_x{0}, m_trans_y{0} {
  ini::IniFile data;
  try {
    data.load(config_path);
  } catch (const std::exception &e) {
    fmt::panic("Failed to load config file: %s", e.what());
  }

  auto &window = data["window"];
  m_width = window["width"].as<int>();
  m_height = window["height"].as<int>();
  size = window["size"].as<double>();
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
  size *= m_scale;
  m_fps_counter = 0;
}

Renderer::~Renderer() {
  SDL_DestroyWindow(m_window);
  m_window = nullptr;
  m_surface = nullptr;
}

std::shared_ptr<Assets> Renderer::get_assets() const { return m_assets; }
double Renderer::get_scale() const { return m_scale; }
double Renderer::get_size() const { return size; }
int Renderer::get_window_width() const { return m_width; }
int Renderer::get_window_height() const { return m_height; }

void Renderer::flip() {
  static Uint64 last_time = 0;
  static const double ms_in_s = 1000;

  Uint64 current_time = SDL_GetTicks64(); // this should be the very first thing
  Uint64 elapsed_time = current_time - last_time;

  double target_delay = ms_in_s / m_fps;
  auto wait_time = target_delay - static_cast<double>(elapsed_time);
  // if (wait_time > 0) SDL_Delay(static_cast<Uint32>(wait_time));
  if (wait_time > 0) {
    std::this_thread::sleep_for(
      std::chrono::milliseconds(static_cast<Uint32>(wait_time)));
  }

  // update window as soon as we delay
  switch (SDL_UpdateWindowSurface(m_window)) {
  case 0: break;
  default: fmt::error("Failed to update window surface: %s", SDL_GetError());
  }
  m_fps_counter++;              // update total frame count
  last_time = SDL_GetTicks64(); // here we can't use current_time, because
                                // SDL_Delay() takes some time too :)
}

void Renderer::clear() {
  switch (
    SDL_FillRect(m_surface, nullptr, SDL_MapRGB(m_surface->format, 0, 0, 0))) {
  case 0: break;
  default: fmt::error("Failed to reset background: %s", SDL_GetError());
  }
}

void Renderer::blit(SDL_Rect src, int x, int y, double scale) {

  auto w = static_cast<double>(src.w * m_scale) * scale;
  auto h = static_cast<double>(src.h * m_scale) * scale;
  auto real_x = static_cast<double>(x);
  auto real_y = static_cast<double>(y);

  switch (m_rect_mode) {
  case RectMode::CENTER: real_x -= w / 2; real_y -= h / 2;
  default:;
  }
  real_x += m_trans_x;
  real_y += m_trans_y;

  SDL_Rect dest = {static_cast<int>(real_x), static_cast<int>(real_y),
                   static_cast<int>(w), static_cast<int>(h)};
  switch (SDL_SetColorKey(m_sprites, true, 0)) {
  case 0: break;
  default: fmt::error("Failed to set color key: %s", SDL_GetError());
  }
  SDL_BlitScaled(m_sprites, &src, m_surface, &dest); // confusing macro
}

void Renderer::text(const std::string &text, int x, int y) {
  // as we do not use sdl_ttf, we have to render the text ourselves
  // we can use sprites for that
  // we have to render each character separately

  static const double custom_scale = 0.7;
  SDL_Rect src = m_assets->get_sprite_alpha_numerical(' ');

  SDL_Rect dest = {static_cast<int>(x), static_cast<int>(y), 0, 0};
  for (auto c : text) {
    switch (c) {
    case '\n':
      dest.x = x;
      dest.y += src.h * m_scale;
      continue;
    }
    src = m_assets->get_sprite_alpha_numerical(c);
    blit(src, dest.x, dest.y, custom_scale);
    dest.x += src.w * m_scale;
  }
}

void Renderer::translate(double x, double y) {
  m_trans_x += x;
  m_trans_y += y;
}

void Renderer::push() {
  m_config_stack.emplace_back(m_rect_mode, m_trans_x, m_trans_y);
}

void Renderer::pop() {
  const struct Config c = m_config_stack.back();
  m_config_stack.pop_back();
  m_rect_mode = c.rect_mode;
  m_trans_x = c.trans_x;
  m_trans_y = c.trans_y;
}

void Renderer::rect_mode(RectMode mode) { m_rect_mode = mode; }

size_t Renderer::get_fps_count() const { return m_fps_counter; }
int Renderer::get_max_fps() const { return m_fps; }
int Renderer::get_update_interval() const { return m_update_interval; }
size_t Renderer::get_fps_anim_count() const {
  return m_fps_counter / m_update_interval;
}
