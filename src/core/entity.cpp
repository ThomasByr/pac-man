
#include <SDL.h>

#include "core/entity.h"

#include "utils.h"

Entity::Entity(const double cx, const double cy, const double w, const double h)
  : m_cx{cx}, m_cy{cy}, w{w}, h{h}, m_speed{1}, m_lives{3}, m_max_lives{3}, m_score{0},
    m_direction{Direction::NONE}, m_reg_direction{Direction::NONE} {
  m_timer = sys_pause::Timer();

  gen = std::mt19937(rd());
  dis = std::uniform_int_distribution<>(10, 30);
}

Direction Entity::get_direction() const { return m_direction; }
unsigned Entity::get_lives() const { return m_lives; }

void Entity::set_direction(const Direction direction) {
  if (direction == Direction::NONE) {
    fmt::debug("Direction::NONE encountered in Entity::set_direction");
  }
  m_reg_direction = direction;
}

std::tuple<int, int> Entity::get_ij(double size) const {
  return std::make_tuple(static_cast<int>(m_cx / size),
                         static_cast<int>(m_cy / size));
}

bool Entity::can_change_direction(std::shared_ptr<Map> map) const {
  const double epsilon = m_speed / 2.05;
  const double tile_size = map->get_size();

  // the relative x position of the entity on the tile
  const double relative_x = std::fmod(m_cx, tile_size);
  // the relative y position of the entity on the tile
  const double relative_y = std::fmod(m_cy, tile_size);

  // if the entity is in the middle of the tile
  return std::abs(relative_x - tile_size / 2) < epsilon &&
         std::abs(relative_y - tile_size / 2) < epsilon;
}

void Entity::teleport(std::shared_ptr<Map> map) {
  const double tile_size = map->get_size();
  const double map_width = map->get_width();

  // if the entity is in the middle of the tile
  if (can_change_direction(map)) {
    // if the entity is on the left most tile
    if (m_cx < tile_size) {
      m_cx = map_width - tile_size;
    }
    // if the entity is on the right side of the map
    else if (m_cx > map_width - tile_size) {
      m_cx = tile_size;
    }
  }
}

bool Entity::ate_entity(double other_cx, double other_cy) const {
  const double epsilon = 10;
  return std::abs(m_cx - other_cx) < epsilon && std::abs(m_cy - other_cy) < epsilon;
}
