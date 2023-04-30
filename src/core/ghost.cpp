
#include <SDL.h>

#include "core/ghost.h"

#include "utils.h"

Ghost::Ghost(const double cx, const double cy, GhostType type, bool is_at_home)
  : Entity{cx, cy, 0, 0}, type{type}, is_at_home{is_at_home} {}

void Ghost::show(std::shared_ptr<Renderer> renderer) {

  SDL_Rect asset;
  static const double custom_scale = 0.6;

  switch (type) {
  case GhostType::BLINKY:
    asset = renderer->get_assets()->get_sprite_ghost_red(
      m_direction, renderer->get_fps_anim_count());
    break;
  case GhostType::PINKY:
    asset = renderer->get_assets()->get_sprite_ghost_pink(
      m_direction, renderer->get_fps_anim_count());
    break;
  case GhostType::INKY:
    asset = renderer->get_assets()->get_sprite_ghost_blue(
      m_direction, renderer->get_fps_anim_count());
    break;
  case GhostType::CLYDE:
    asset = renderer->get_assets()->get_sprite_ghost_orange(
      m_direction, renderer->get_fps_anim_count());
    break;
  }

  renderer->push();
  renderer->rect_mode(RectMode::CENTER);
  renderer->translate(m_cx, m_cy);
  renderer->blit(asset, 0, 0, custom_scale);
  renderer->pop();
}

bool Ghost::can_go(std::shared_ptr<Map> map, const Direction &dir) const {
  int i = -1, j = -1;
  std::tie(i, j) = get_ij(map->get_size());
  if (i == -1 || j == -1) { return false; }

  return map->can_go(j, i, dir); // why j, i and not i, j ?
                                 // well ...
}

bool Ghost::can_change_direction(std::shared_ptr<Map> map) const {
  static const double epsilon = 0.01;
  double tile_size = map->get_size();

  // the relative x position of the entity on the tile
  double relative_x = std::fmod(m_cx, tile_size);
  // the relative y position of the entity on the tile
  double relative_y = std::fmod(m_cy, tile_size);

  // if the entity is in the middle of the tile
  return std::abs(relative_x - tile_size / 2) < epsilon &&
         std::abs(relative_y - tile_size / 2) < epsilon;
}

void Ghost::blinky_chase(std::shared_ptr<Map> map,
                         std::tuple<int, int> pacman_pos) {

  Node pacman_tile = {std::get<0>(pacman_pos), std::get<1>(pacman_pos), 0, 0};

  std::tuple<int, int> ghost_pos = get_ij(map->get_size());

  Node ghost_tile = {std::get<0>(ghost_pos), std::get<1>(ghost_pos), 0, 0};

  m_direction = map->shortest_path(pacman_tile, ghost_tile);
}

void Ghost::pinky_chase(std::shared_ptr<Map> map,
                        std::tuple<int, int> pacman_pos) {
  (void)pacman_pos;
  (void)map;
}

void Ghost::inky_chase(std::shared_ptr<Map> map,
                       std::tuple<int, int> pacman_pos) {
  (void)pacman_pos;
  (void)map;
}

void Ghost::clyde_chase(std::shared_ptr<Map> map,
                        std::tuple<int, int> pacman_pos) {
  (void)pacman_pos;
  (void)map;
}

void Ghost::chase_pacman(std::shared_ptr<Map> map,
                         std::tuple<int, int> pacman_pos) {
  switch (type) {
  case GhostType::BLINKY: blinky_chase(map, pacman_pos); break;
  case GhostType::PINKY: break;
  case GhostType::INKY: break;
  case GhostType::CLYDE: break;
  default: break;
  }
}

void Ghost::update(std::shared_ptr<Map> map, std::tuple<int, int> Pacman_pos) {

  if (is_at_home) {

    if (m_direction == Direction::NONE) { m_direction = m_reg_direction; }

    // first check if we can still go in the current direction
    if (!can_go(map, m_direction)) {
      // in that case we simply stop
      m_direction = opposite(m_direction);
      auto pos = get_ij(map->get_size());
      fmt::debug("type %i : %i %i", type, std::get<0>(pos), std::get<1>(pos));
    }

  }

  else {
    chase_pacman(map, Pacman_pos);

    if (m_reg_direction != Direction::NONE &&
        m_direction == opposite(m_reg_direction)) {
      m_direction = m_reg_direction;
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
  }

  switch (m_direction) {
  case Direction::UP: m_cy -= 1; break;
  case Direction::DOWN: m_cy += 1; break;
  case Direction::LEFT: m_cx -= 1; break;
  case Direction::RIGHT: m_cx += 1; break;

  default: break;
  }

  m_reg_direction = m_direction;
}
