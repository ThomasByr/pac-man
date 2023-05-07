
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

  void show(std::shared_ptr<Renderer> renderer);
  size_t get_score(void) const;

  bool can_go(std::shared_ptr<Map> map, const Direction &dir) const;

  bool ate_food(std::shared_ptr<Map> map);
  void eat_food(std::shared_ptr<Map> map);
  bool ate_big_food(std::shared_ptr<Map> map);
  void eat_big_food(std::shared_ptr<Map> map);

  void update(std::shared_ptr<Map> map);
  void move(std::shared_ptr<Map> map);

  bool eat_entity();
  bool ate_all_dots(void) const;
  bool play_dead(std::shared_ptr<Renderer> renderer) const;

  /**
   * @brief reset pacman to its initial state
   * 
   * @param go if true, pacman will recover its entire life
   */
  void reset(bool go = false);
  bool is_powered(void) const;
};

#endif // __inc_core_pcmn_H__
