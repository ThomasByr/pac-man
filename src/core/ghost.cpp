
#include <SDL.h>

#include "core/ghost.h"

Ghost::Ghost(const double cx, const double cy, const double w, const double h,
             GhostType type)
  : Entity{cx, cy, w, h}, type{type} {}

void Ghost::show(std::shared_ptr<Renderer> renderer) {

  SDL_Rect asset;

  switch (type) {
  case GhostType::BLINKY:
    asset = renderer->get_assets()->get_sprite_ghost_red(m_direction, 0);
    break;
  case GhostType::PINKY:
    asset = renderer->get_assets()->get_sprite_ghost_pink(m_direction, 0);
    break;
  case GhostType::INKY:
    asset = renderer->get_assets()->get_sprite_ghost_blue(m_direction, 0);
    break;
  case GhostType::CLYDE:
    asset = renderer->get_assets()->get_sprite_ghost_orange(m_direction, 0);
    break;
  }

  renderer->push();
  renderer->rect_mode(RectMode::CENTER);
  renderer->translate(m_cx, m_cy);
  renderer->blit(asset, 0, 0);
  renderer->pop();
}
