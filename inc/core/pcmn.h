
#ifndef __inc_core_pcmn_H__
#define __inc_core_pcmn_H__

#include <SDL.h>

#include <memory>

#include "helper/renderer.h"
#include "state.h"

#include "entity.h"

struct PacmanConfig {
  int m_points_per_dot, m_points_per_power_dot;
  int m_points_per_ghost;
  int max_number_of_dots;
};

class Pacman : public Entity {
private:
  int m_points_per_dot, m_points_per_power_dot;
  int m_points_per_ghost;
  int max_number_of_dots;
  int m_dots_eaten;
  PcmnState state;

public:
  Pacman(const double cx, const double cy, const struct PacmanConfig &config);
  Pacman(const Pacman &other) = delete;
  Pacman(Pacman &&other) = delete;
  Pacman &operator=(const Pacman &other) = delete;
  Pacman &operator=(Pacman &&other) = delete;

  virtual ~Pacman() = default;

  /**
   * @brief show pacman on the screen
   * 
   * @param renderer renderer to use
   */
  void show(std::shared_ptr<Renderer> renderer) override;
  /// @brief get pacman score
  size_t get_score(void) const;

  /**
   * @brief return true if pacman can go in the given direction
   * 
   * @param map map to use
   * @param dir direction to test
   * @return true  pacman can go in the given direction
   * @return false otherwise
   */
  bool can_go(std::shared_ptr<Map> map, const Direction &dir) const override;

  /// @brief return true if pacman ate a dot
  bool ate_food(std::shared_ptr<Map> map);
  /// @brief eat a dot
  void eat_food(std::shared_ptr<Map> map);
  /// @brief return true if pacman ate a big dot
  bool ate_big_food(std::shared_ptr<Map> map);
  /// @brief eat a big dot (and power up pacman)
  void eat_big_food(std::shared_ptr<Map> map);
  /// @brief return true if pacman ate a fruit
  bool ate_fruit(std::shared_ptr<Map> map);
  /// @brief eat a fruit
  void eat_fruit(std::shared_ptr<Map> map);

  /// @brief main update function (does not deal with ghost interaction)
  void update(std::shared_ptr<Map> map);
  /// @brief move the entity on the map
  void move(std::shared_ptr<Map> map) override;

  /// @brief eat a ghost (panic if pacman is not powered)
  void eat_ghost(void);
  /// @brief return true if pacman ate all dots
  bool ate_all_dots(void) const;
  /// @brief play dead animation (return true if animation is over)
  bool play_dead(std::shared_ptr<Renderer> renderer) const;

  /**
   * @brief reset pacman to its initial state
   * 
   * @param g_o game over : if true, pacman will recover its entire life
   * @param e_l end level : if true, pacman dots eaten will be reset
   */
  void reset(bool g_o = false, bool e_l = false);
  /**
   * @brief return true if pacman is powered
   * 
   * @return true  pacman is powered
   * @return false otherwise
   */
  bool is_powered(void) const;
};

#endif // __inc_core_pcmn_H__
