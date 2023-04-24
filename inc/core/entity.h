
#ifndef __inc_core_entity_H__
#define __inc_core_entity_H__

#include <SDL.h>

#include <memory>
#include <tuple>

#include "core/map.h"
#include "helper/renderer.h"

#include "directions.h"

class Entity {
protected:
  double m_cx, m_cy; // center position of the entity on the map
  double w, h;       // width and height of the entity

  Direction m_direction, m_reg_direction;

public:
  Entity(const double cx, const double cy, const double w, const double h);
  virtual void show(std::shared_ptr<Renderer> renderer) = 0;

  // virtual void update() = 0;

  virtual void set_direction(const Direction direction);

  /**
   * @brief Get the i and j of the entity on the map (index of the tile)
   *
   * @param size size of the tile
   * @return std::tuple<int, int>
   */
  std::tuple<int, int> get_ij(double size) const;
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
  virtual bool can_change_direction(std::shared_ptr<Map> map) const = 0;
};

#endif // __inc_core_entity_H__
