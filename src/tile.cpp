
#include "tile.h"

Tile::Tile(const int type, const int x, const int y) {
  m_type = static_cast<TileType>(type);
  m_x = x;
  m_y = y;
}

void Tile::show(SDL_Surface *surface) {
  (void)surface;
  switch (m_type) {

  case TileType::DOT:
    // todo: draw a dot
    break;
  case TileType::POWER_PELLET:
    // todo: draw a power pellet
    break;

  default: break;
  }
}
