
#ifndef __inc_helper_assets_H__
#define __inc_helper_assets_H__

#include <SDL.h>

#include <map>
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

  /* alpha-numerical characters */
  SDL_Rect m_alpha_numerical_default;
  std::map<char, SDL_Rect> m_alpha_numerical;

  /* a bunch of rectangles for the sprites */

  SDL_Rect m_dot;       // dot
  SDL_Rect m_power_dot; // power dot

  std::vector<SDL_Rect> m_pacman_up;    // facing up
  std::vector<SDL_Rect> m_pacman_down;  // facing down
  std::vector<SDL_Rect> m_pacman_left;  // facing left
  std::vector<SDL_Rect> m_pacman_right; // facing right

  SDL_Rect m_pacman_closed;            // closed mouth
  std::vector<SDL_Rect> m_pacman_dead; // dead pacman

  std::vector<SDL_Rect> m_redghost_right; // red ghost right
  std::vector<SDL_Rect> m_redghost_left;  // facing left
  std::vector<SDL_Rect> m_redghost_up;    // facing up
  std::vector<SDL_Rect> m_redghost_down;  // facing down

  std::vector<SDL_Rect> m_pinkghost_right; // pink ghost
  std::vector<SDL_Rect> m_pinkghost_left;
  std::vector<SDL_Rect> m_pinkghost_up;
  std::vector<SDL_Rect> m_pinkghost_down;

  std::vector<SDL_Rect> m_cyanghost_right; // blue ghost
  std::vector<SDL_Rect> m_cyanghost_left;
  std::vector<SDL_Rect> m_cyanghost_up;
  std::vector<SDL_Rect> m_cyanghost_down;

  std::vector<SDL_Rect> m_orangeghost_right; // orange ghost
  std::vector<SDL_Rect> m_orangeghost_left;
  std::vector<SDL_Rect> m_orangeghost_up;
  std::vector<SDL_Rect> m_orangeghost_down;

  // todo add vector for direction

  std::vector<SDL_Rect> m_weak_ghost; // frightened ghost

  std::vector<SDL_Rect> m_weak_ghost_ending; // frightened ghost blinking

  std::vector<SDL_Rect> m_eyes_right; // dead ghost eye
  std::vector<SDL_Rect> m_eyes_left;  // facing left...
  std::vector<SDL_Rect> m_eyes_up;
  std::vector<SDL_Rect> m_eyes_down;

public:
  SDL_Rect m_bg; // background

  /**
   * @brief Construct a new Assets object
   *
   * @param path the path to the assets
   */
  Assets(const std::string &path = "assets/pacman_sprites.bmp");
  ~Assets();

  /**
   * @brief Get the sprite alpha numerical object
   *
   * @param c the character
   * @return SDL_Rect - the sprite
   */
  SDL_Rect get_sprite_alpha_numerical(char c) const;

  SDL_Surface *get_surface(void) const;

  SDL_Rect get_sprite_dot(void) const;
  SDL_Rect get_sprite_power_dot(void) const;

  /**
   * @brief Get the Sprite object for pacman
   *
   * @param dir the direction of the sprite
   * @param fc the frame count
   * @return SDL_Rect - the sprite
   */
  SDL_Rect get_sprite_pacman(const Direction &dir, int fc) const;

  /**
   * @brief Get the Sprite object for ghost red
   *
   * @param dir the direction of the sprite
   * @param fc the frame count
   * @return SDL_Rect - the sprite
   */
  SDL_Rect get_sprite_ghost_red(const Direction &dir, int fc) const;
  /**
   * @brief Get the Sprite object for ghost pink
   *
   * @param dir the direction of the sprite
   * @param fc the frame count
   * @return SDL_Rect - the sprite
   */
  SDL_Rect get_sprite_ghost_pink(const Direction &dir, int fc) const;
  /**
   * @brief Get the Sprite object for ghost blue
   *
   * @param dir the direction of the sprite
   * @param fc the frame count
   * @return SDL_Rect - the sprite
   */
  SDL_Rect get_sprite_ghost_blue(const Direction &dir, int fc) const;
  /**
   * @brief Get the Sprite object for ghost orange
   *
   * @param dir the direction of the sprite
   * @param fc the frame count
   * @return SDL_Rect - the sprite
   */
  SDL_Rect get_sprite_ghost_orange(const Direction &dir, int fc) const;
};

#endif // __inc_helper_assets_H__
