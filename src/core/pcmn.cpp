
#include <SDL.h>

#include "core/pcmn.h"

#include "utils.h"

Pacman::Pacman(const double cx, const double cy,
               const struct PacmanConfig &config)
  : Entity{cx, cy, 0, 0}, m_points_per_dot{config.m_points_per_dot},
    m_points_per_power_dot{config.m_points_per_power_dot},
    m_points_per_ghost{config.m_points_per_ghost} {}

void Pacman::show(std::shared_ptr<Renderer> renderer) {
  static const double custom_scale = 0.5;
  const SDL_Rect asset = renderer->get_assets()->get_sprite_pacman(
    m_direction, renderer->get_fps_anim_count());

  renderer->push();
  renderer->rect_mode(RectMode::CENTER);
  renderer->translate(m_cx, m_cy);
  renderer->blit(asset, 0, 0, custom_scale);
  renderer->pop();
}

size_t Pacman::get_score() const { return m_score; }

bool Pacman::can_go(std::shared_ptr<Map> map, const Direction &dir) const {
  int i = -1, j = -1;
  std::tie(i, j) = get_ij(map->get_size());
  if (i == -1 || j == -1) { return false; }

  return map->can_go(j, i, dir); // why j, i and not i, j ?
                                 // well ...
}

bool Pacman::can_change_direction(std::shared_ptr<Map> map) const {
  static const double epsilon = .5;
  double tile_size = map->get_size();

  // the relative x position of the entity on the tile
  double relative_x = std::fmod(m_cx, tile_size);
  // the relative y position of the entity on the tile
  double relative_y = std::fmod(m_cy, tile_size);

  // if the entity is in the middle of the tile
  return std::abs(relative_x - tile_size / 2) < epsilon &&
         std::abs(relative_y - tile_size / 2) < epsilon;
}

bool Pacman::ate_food(std::shared_ptr<Map> map) {
  int i = -1, j = -1;
  std::tie(i, j) = get_ij(map->get_size());
  if (i == -1 || j == -1) { return false; }

  return map->ate_food(j, i); // why j, i and not i, j ? well ...
}

void Pacman::eat_food(std::shared_ptr<Map> map) {
  int i = -1, j = -1;
  std::tie(i, j) = get_ij(map->get_size());
  if (i == -1 || j == -1) { return; }

  map->eat_food(j, i); // those damn vectors

  m_score += m_points_per_dot;
}

void Pacman::update(std::shared_ptr<Map> map) {

  // first check if we can still go in the current direction
  if (can_change_direction(map) && !can_go(map, m_direction)) {
    // in that case we simply stop
    m_direction = Direction::NONE;
  }

  // then we check if the registered direction is valid
  // we do not want to check if the current direction is NONE because we can
  // change direction while in motion
  if (can_change_direction(map) && can_go(map, m_reg_direction)) {
    if (m_reg_direction != m_direction) {
      m_direction = m_reg_direction;
      m_reg_direction = Direction::NONE;
    }
  }

  // finally we can check if we can go in the opposite direction
  // even if we are not in the middle of the tile
  if (m_direction != Direction::NONE &&
      m_reg_direction == opposite(m_direction) &&
      can_go(map, opposite(m_direction))) {
    m_direction = opposite(m_direction);
  }

  switch (m_direction) {
  case Direction::UP: m_cy -= 1; break;
  case Direction::DOWN: m_cy += 1; break;
  case Direction::LEFT: m_cx -= 1; break;
  case Direction::RIGHT: m_cx += 1; break;
  default: break;
  }
  if (ate_food(map)) { eat_food(map); }
}
