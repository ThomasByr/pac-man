
#ifndef __inc_core_entity_H__
#define __inc_core_entity_H__

#include <SDL.h>

#include <memory>
#include <random>
#include <tuple>

#include "core/map.h"
#include "helper/renderer.h"

#include "directions.h"
#include "utils.h"

class Entity {
protected:
  double m_start_cx, m_start_cy; // start position of the entity on the map
  double m_cx, m_cy;             // center position of the entity on the map
  double w, h;                   // width and height of the entity
  double m_speed;                // speed of the entity

  unsigned m_lives, m_max_lives; // lives of the entity
  std::random_device rd;         // random device to generate random numbers
  std::mt19937 gen;              // random generator

  std::uniform_int_distribution<> dis; // random distribution

  size_t m_score; // score of the entity (only makes sense for pacman ?)

  Direction m_direction, m_reg_direction;
  sys_pause::Timer m_timer; // timer to control the speed behavior

  // global timer (will be updated in the main loop)
  std::shared_ptr<sys_pause::Timer> p_timer;

public:
  Entity(const double cx, const double cy, const double w, const double h);
  virtual void show(std::shared_ptr<Renderer> renderer) = 0;

  void set_direction(const Direction direction);

  /// @brief get the current direction of the entity (not the registered one)
  Direction get_direction(void) const;

  /// @brief the number of lives of the entity
  unsigned get_lives(void) const;

  /**
   * @brief Get the i and j of the entity on the map (index of the tile)
   *
   * @param size size of the tile
   * @return std::tuple<int, int>
   */
  std::tuple<int, int> get_ij(double size) const;
  /**
   * @brief Get the real position of the entity on the map (double precision)
   * 
   * @return std::tuple<double, double> 
   */
  std::tuple<double, double> get_pos(void) const;
  /**
   * @brief Check if the entity can go to the direction
   *
   * @param map map of the game
   * @param dir direction to check
   * @return true if the entity can go to the direction ;
   * @return false otherwise
   */
  virtual bool can_go(std::shared_ptr<Map> map, const Direction &dir) const = 0;
  /**
   * @brief Check if the entity can change direction
   * @note Does not check if the entity can go to any direction, only if it is
   * in the middle of a tile
   *
   * @param map map of the game
   * @return true if the entity is somewhere in the middle of a tile
   * @return false otherwise
   */
  bool can_change_direction(std::shared_ptr<Map> map) const;
  /// @brief do the entity teleportation
  void teleport(std::shared_ptr<Map> map);
  /// @brief move the entity
  virtual void move(std::shared_ptr<Map> map) = 0;

  /**
   * @brief whether the entity ate the other entity
   *
   * @param other_cx other entity center x
   * @param other_cy other entity center y
   * @return true    if the entity ate the other entity
   * @return false   otherwise
   */
  bool ate_entity(double other_cx, double other_cy) const;
  /// @brief if the entity life is 0
  bool is_dead(void) const;
  /// @brief die ... just die (remove one life)
  void die(void);

  /**
   * @brief add some time to the internal timer
   * 
   * @param sec seconds to add
   */
  void add_timer_time(unsigned sec);
};

#endif // __inc_core_entity_H__
