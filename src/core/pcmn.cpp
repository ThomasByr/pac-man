
#include "core/pcmn.h"

Pacman::Pacman(const double cx, const double cy, const double w, const double h) : Entity{cx, cy, w, h} {}

void Pacman::show(std::shared_ptr<Renderer> renderer) {
  (void)renderer;
  throw "Not implemented";
}
