
#ifndef __inc_helper_renderer_H__
#define __inc_helper_renderer_H__

#include <SDL2/SDL.h>

#include <memory>
#include <string>
#include <vector>

#include "helper/assets.h"
#include "interface/showable.h"

enum class RectMode {CENTER, CORNER};

class Renderer {
private:
  SDL_Window *m_window;   // the window
  SDL_Surface *m_surface; // surface of the window
  SDL_Surface *m_sprites; // surface of the sprites
  double m_scale;         // scale of the window

  RectMode m_rect_mode; // rect mode

  int m_width;       // width of the window
  int m_height;      // height of the window
  bool m_fullscreen; // fullscreen or not

  int m_fps; // frames per second
  int m_ups; // updates per second

  int m_update_interval; // interval between animation updates

  std::vector<std::shared_ptr<Showable>> m_showables;

  std::shared_ptr<Assets> m_assets;

public:
  Renderer(const std::string &title, const std::string &config_path = ".ini");
  ~Renderer();

  std::shared_ptr<Assets> get_assets(void) const;

  void add_showable(std::shared_ptr<Showable> showable);
  void remove_showable(std::shared_ptr<Showable> showable);

  void flip(void);
  void clear(void);
  void blit(SDL_Rect src, int x, int y);

  void text(const std::string &text, int x, int y);

  void rect_mode(RectMode mode);
};

#endif // __inc_helper_renderer_H__
