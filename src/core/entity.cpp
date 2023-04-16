
#include <SDL.h>

#include "core/entity.h"

Entity::Entity(const double cx, const double cy, const double w, const double h)

    : m_cx{cx}, m_cy{cy}, w{w}, h{h}, m_direction{Direction::NONE}, m_reg_direction{Direction::NONE} {}

void Entity::set_direction(const Direction direction) {
  m_reg_direction = direction;
}
