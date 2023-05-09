
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

  std::vector<SDL_Rect> m_weak_ghost; // frightened ghost

  std::vector<SDL_Rect> m_weak_ghost_ending; // frightened ghost blinking

  SDL_Rect m_eyes_right; // dead ghost eye facing right
  SDL_Rect m_eyes_left;  // dead ghost eye facing left
  SDL_Rect m_eyes_up;    // dead ghost eye facing up
  SDL_Rect m_eyes_down;  // dead ghost eye facing down

public:
  SDL_Rect m_bg, m_bg_menu; // background
  SDL_Rect m_lives;         // life indicator

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
   * @brief Get the Sprite object for pacman dead
   *
   * @param fc  the frame count
   * @param end becomes true when the animation is over
   * @return SDL_Rect - the sprite
   */
  SDL_Rect get_sprite_pacman_dead(int fc, int &end) const;

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

  /**
   * @brief Get the sprite object for ghost weak
   * 
   * @param fc the frame count
   * @param ending if the ghost should blink
   * @return SDL_Rect - the sprite
   */
  SDL_Rect get_sprite_ghost_weak(int fc, bool ending = false) const;
  /**
   * @brief Get the sprite object for ghost eyes
   * 
   * @param dir direction of the ghost
   * @return SDL_Rect - the sprite
   */
  SDL_Rect get_sprite_ghost_eyes(const Direction &dir) const;
};

#endif // __inc_helper_assets_H__
