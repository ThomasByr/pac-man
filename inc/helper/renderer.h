
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

  Config(enum RectMode rect_mode, double trans_x, double trans_y)
    : rect_mode{rect_mode}, trans_x{trans_x}, trans_y{trans_y} {}
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

  /**
   * @brief Get the assets object
   *
   * @return std::shared_ptr<Assets>
   */
  std::shared_ptr<Assets> get_assets(void) const;
  /// @brief get the base scale of the renderer (not the custom scale)
  double get_scale(void) const;
  /// @brief get the size of a tile
  double get_size(void) const;
  /// @brief width of the window
  int get_window_width(void) const;
  /// @brief height of the window
  int get_window_height(void) const;

  /// @brief flip double buffer and wait to reach the target fps
  void flip(void);
  /// @brief clear the screen (apply dark background)
  void clear(void);
  /**
   * @brief blit a sprite on the screen
   *
   * @param src   source rectangle
   * @param x     destination x
   * @param y     destination y
   * @param scale custom scale
   */
  void blit(SDL_Rect src, int x, int y, double scale = 1);

  /**
   * @brief write some text to the screen
   *
   * @param text text to write
   * @param x    x position
   * @param y    y position
   */
  void text(const std::string &text, int x, int y);

  /**
   * @brief set the rect mode
   *
   * @param mode CORNER or CENTER
   */
  void rect_mode(RectMode mode);
  /**
   * @brief translate the origin of the screen
   *
   * @param x x offset
   * @param y y offset
   */
  void translate(double x = 0, double y = 0);
  /// @brief push the current configuration to the stack
  void push(void);
  /// @brief pop the last configuration from the stack
  void pop(void);

  /// @brief get the current fps count
  size_t get_fps_count(void) const;
  /// @brief get the maximum (target) fps
  int get_max_fps(void) const;
  /// @brief get the animation update interval
  int get_update_interval(void) const;
  /// @brief get the fps count, scaled down by the update interval
  size_t get_fps_anim_count(void) const;
};

#endif // __inc_helper_renderer_H__
