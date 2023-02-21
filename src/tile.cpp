
#include <SDL2/SDL.h>

#include <memory>

#include "tile.h"

#include "helper/assets.h"
#include "helper/renderer.h"

#include "utils.h"

Tile::Tile(const int type, const int x, const int y, const double size) {
  m_type = static_cast<TileType>(type);
  m_i = x;
  m_j = y;
  this->size = size;
}

void Tile::show(std::shared_ptr<Renderer> renderer) {

  renderer->push();
  renderer->rect_mode(RectMode::CENTER);
  double t = std::floor(size / 2.0);
  renderer->translate(t - 1, t - 1);

  double x = m_j * size;
  double y = m_i * size;

  switch (m_type) {

  case TileType::DOT:
    renderer->blit(renderer->get_assets()->get_sprite_dot(),
                   static_cast<int>(x), static_cast<int>(y));
    break;
  case TileType::POWER_DOT:
    renderer->blit(renderer->get_assets()->get_sprite_power_dot(),
                   static_cast<int>(x), static_cast<int>(y));
    break;

  default: break;
  }
  renderer->pop();
}
