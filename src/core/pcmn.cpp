
#include <SDL2/SDL.h>

#include "core/pcmn.h"

Pacman::Pacman(const double cx, const double cy, const double w, const double h) : Entity{cx, cy, w, h} {}

void Pacman::show(std::shared_ptr<Renderer> renderer) {
  SDL_Rect asset = renderer->get_assets()->get_sprite_pacman(m_direction, 0);

  renderer->push();
  renderer->rect_mode(RectMode::CENTER);
  renderer->translate(m_cx, m_cy);
  renderer->blit(asset, 0, 0);
  renderer->pop();
}
