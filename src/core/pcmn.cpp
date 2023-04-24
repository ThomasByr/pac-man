
#include <SDL.h>

#include "core/pcmn.h"

#include "utils.h"

Pacman::Pacman(const double cx, const double cy, const double w, const double h)
  : Entity{cx, cy, w, h} {}

void Pacman::show(std::shared_ptr<Renderer> renderer) {
  const SDL_Rect asset = renderer->get_assets()->get_sprite_pacman(
    m_direction, renderer->get_fps_count() / renderer->get_update_interval());

  renderer->push();
  renderer->rect_mode(RectMode::CENTER);
  renderer->translate(m_cx, m_cy);
  renderer->blit(asset, 0, 0, .5);
  renderer->pop();
}

bool Pacman::can_go(const Map &map, const Direction &dir) const {
  int i = -1, j = -1;
  std::tie(i, j) = get_ij(map.get_size());
  if (i == -1 || j == -1) { return false; }

  return map.can_go(i, j, dir);
}

bool Pacman::can_change_direction(const Map &map) const {
  static const double epsilon = 1e-1;
  double tile_size = map.get_size();

  // the relative x position of the entity on the tile
  double relative_x = std::fmod(m_cx, tile_size);
  // the relative y position of the entity on the tile
  double relative_y = std::fmod(m_cy, tile_size);

  // if the entity is in the middle of the tile
  return std::abs(relative_x - tile_size / 2) < epsilon &&
         std::abs(relative_y - tile_size / 2) < epsilon;
}
