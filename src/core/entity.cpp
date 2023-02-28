
#include <SDL2/SDL.h>

#include "core/entity.h"

Entity::Entity(const double cx, const double cy, const double w, const double h)
    : m_cx{cx}, m_cy{cy}, w{w}, h{h} {}
