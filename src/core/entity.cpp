
#include <SDL.h>

#include "core/entity.h"

#include "utils.h"

Entity::Entity(const double cx, const double cy, const double w, const double h)

  : m_cx{cx}, m_cy{cy}, w{w}, h{h}, m_direction{Direction::NONE},
    m_reg_direction{Direction::NONE} {}

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
