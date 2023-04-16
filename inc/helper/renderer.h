
#ifndef __inc_helper_renderer_H__
#define __inc_helper_renderer_H__

#include <SDL.h>

#include <memory>
#include <string>
#include <vector>

#include "helper/assets.h"

enum class RectMode { CENTER, CORNER };

struct Config {
  RectMode rect_mode;
  double trans_x, trans_y;
};

class Renderer {
private:
  SDL_Window *m_window;   // the window
  SDL_Surface *m_surface; // surface of the window
  SDL_Surface *m_sprites; // surface of the sprites
  double m_scale;         // scale of the window
  double size;            // size of a tile

  size_t m_fps_counter; // counter for fps

  std::vector<struct Config> m_config_stack{}; // stack of configurations

  RectMode m_rect_mode;        // rect mode
  double m_trans_x, m_trans_y; // translation

  int m_width;       // width of the window
  int m_height;      // height of the window
  bool m_fullscreen; // fullscreen or not

  int m_fps; // frames per second
  int m_ups; // updates per second

  int m_update_interval; // interval between animation updates

  std::shared_ptr<Assets> m_assets;

public:
  Renderer(const std::string &title, const std::string &config_path = ".ini");
  ~Renderer();

  std::shared_ptr<Assets> get_assets(void) const;
  double get_scale(void) const;
  double get_size(void) const;
  int get_window_width(void) const;
  int get_window_height(void) const;

  void flip(void);
  void clear(void);
  void blit(SDL_Rect src, int x, int y, double scale = 1);

  void text(const std::string &text, int x, int y);

  void rect_mode(RectMode mode);
  void translate(double x = 0, double y = 0);
  void push(void);
  void pop(void);

  size_t get_fps_count(void) const;
};

#endif // __inc_helper_renderer_H__
