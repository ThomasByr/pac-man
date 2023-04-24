
#include <SDL.h>

#include "core/ghost.h"

Ghost::Ghost(const double cx, const double cy, const double w, const double h,
             GhostType type)
  : Entity{cx, cy, w, h}, type{type} {}

void Ghost::show(std::shared_ptr<Renderer> renderer) {
  (void)renderer;
  throw "Not implemented";
}
