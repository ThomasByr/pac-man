
#include <SDL.h>

#include <memory>

#include "tile.h"

#include "helper/assets.h"
#include "helper/renderer.h"

#include "utils.h"

Tile::Tile(const int type, const int x, const int y, const double size)
  : m_type{static_cast<TileType>(type)}, m_i{x}, m_j{y}, size{size} {}

void Tile::show(std::shared_ptr<Renderer> renderer) {

  renderer->push();
  renderer->rect_mode(RectMode::CENTER);
  double t = std::floor(size / 2);
  renderer->translate(t - 1, t - 1);

  SDL_Rect assets;

  double x = m_j * size;
  double y = m_i * size;

  switch (m_type) {

  case TileType::DOT: assets = renderer->get_assets()->get_sprite_dot(); break;
  case TileType::POWER_DOT:
    assets = renderer->get_assets()->get_sprite_power_dot();
    break;

  default: break;
  }
  renderer->blit(assets, static_cast<int>(x), static_cast<int>(y));
  renderer->pop();
}

TileType Tile::get_type() const { return m_type; }
void Tile::set_type(const TileType &type) { m_type = type; }

bool Tile::can_go(const std::optional<Tile> &target) const {

  TileType target_type = TileType::WALL; // init to something we can't go to
  if (target.has_value()) { target_type = target.value().m_type; }

  return target_type != TileType::WALL && target_type != TileType::GHOST_HOUSE_DOOR;
}
