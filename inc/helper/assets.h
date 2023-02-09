
#ifndef __inc_helper_assets_H__
#define __inc_helper_assets_H__

#include <SDL2/SDL.h>

#include <string>
#include <vector>

#include "directions.h"

/**
 * @brief all of the game assets
 *
 */
class Assets {
private:
  // the surface of the assets
  SDL_Surface *m_surface;

  /* a bunch of rectangles for the sprites */

  std::vector<SDL_Rect> m_pacman_up;    // facing up
  std::vector<SDL_Rect> m_pacman_down;  // facing down
  std::vector<SDL_Rect> m_pacman_left;  // facing left
  std::vector<SDL_Rect> m_pacman_right; // facing right

  SDL_Rect m_pacman_closed;            // closed mouth
  std::vector<SDL_Rect> m_pacman_dead; // dead pacman

  std::vector<SDL_Rect> m_ghost_red;    // red ghost
  std::vector<SDL_Rect> m_ghost_pink;   // pink ghost
  std::vector<SDL_Rect> m_ghost_blue;   // blue ghost
  std::vector<SDL_Rect> m_ghost_orange; // orange ghost

  std::vector<SDL_Rect> m_ghost_frightened; // frightened ghost
  std::vector<SDL_Rect> m_ghost_dead_eye;   // dead ghost eye

public:
  /**
   * @brief Construct a new Assets object
   *
   * @param path the path to the assets
   */
  Assets(const std::string &path = "assets/pacman_sprites.bmp");
  ~Assets();

  /**
   * @brief Get the Sprite object for pacman
   *
   * @param dir the direction of the sprite
   * @param fc the frame count (0..=2)
   * @return SDL_Rect - the sprite
   */
  SDL_Rect get_sprite_pacman(const Direction &dir, int fc) const;

  /**
   * @brief Get the Sprite object for ghost red
   *
   * @param dir the direction of the sprite
   * @param fc the frame count (0..=1)
   * @return SDL_Rect - the sprite
   */
  SDL_Rect get_sprite_ghost_red(const Direction &dir, int fc) const;
  /**
   * @brief Get the Sprite object for ghost pink
   *
   * @param dir the direction of the sprite
   * @param fc the frame count (0..=1)
   * @return SDL_Rect - the sprite
   */
  SDL_Rect get_sprite_ghost_pink(const Direction &dir, int fc) const;
  /**
   * @brief Get the Sprite object for ghost blue
   *
   * @param dir the direction of the sprite
   * @param fc the frame count (0..=1)
   * @return SDL_Rect - the sprite
   */
  SDL_Rect get_sprite_ghost_blue(const Direction &dir, int fc) const;
  /**
   * @brief Get the Sprite object for ghost orange
   *
   * @param dir the direction of the sprite
   * @param fc the frame count (0..=1)
   * @return SDL_Rect - the sprite
   */
  SDL_Rect get_sprite_ghost_orange(const Direction &dir, int fc) const;
};

#endif // __inc_helper_assets_H__
