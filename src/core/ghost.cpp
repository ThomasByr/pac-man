
#include <SDL.h>

#include "core/ghost.h"

#include "utils.h"

Ghost::Ghost(const double cx, const double cy, GhostType type, bool is_at_home)
  : Entity{cx, cy, 0, 0}, type{type}, is_at_home{is_at_home} {
  if (is_at_home) {
    m_direction = Direction::UP;
  } else {
    m_direction = Direction::LEFT;
  }
}

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
  static const double epsilon = 0.2;
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
                         std::tuple<int, int> pacman_pos,
                         Direction pacman_dir) {

  (void)pacman_dir;

  Node pacman_tile = {std::get<1>(pacman_pos), std::get<0>(pacman_pos)};
  auto [i, j] = get_ij(map->get_size());
  Node ghost_tile = {j, i};

  // m_reg_direction = map->astar(ghost_tile, pacman_tile);
  m_reg_direction = map->stupid(ghost_tile, pacman_tile, m_direction);
}

void Ghost::pinky_chase(std::shared_ptr<Map> map,
                        std::tuple<int, int> pacman_pos, Direction pacman_dir) {

  Node pacman_tile = {std::get<1>(pacman_pos), std::get<0>(pacman_pos)};
  auto [i, j] = get_ij(map->get_size());
  Node ghost_tile = {j, i};

  switch (pacman_dir) {
  case Direction::UP:
    if (map->can_go(pacman_tile.i - 3, pacman_tile.j, pacman_dir)) {
      pacman_tile.i = pacman_tile.i - 3;
      m_reg_direction = map->astar(ghost_tile, pacman_tile);

    } else {
      m_reg_direction = map->astar(ghost_tile, pacman_tile);
    }
    return;
  case Direction::DOWN:
    if (map->can_go(pacman_tile.i + 3, pacman_tile.j, pacman_dir)) {
      pacman_tile.i = pacman_tile.i + 3;
      m_reg_direction = map->astar(ghost_tile, pacman_tile);
    } else {
      m_reg_direction = map->astar(ghost_tile, pacman_tile);
    }
    return;
  case Direction::LEFT:
    if (map->can_go(pacman_tile.i, pacman_tile.j - 3, pacman_dir)) {
      pacman_tile.i = pacman_tile.j - 3;
      m_reg_direction = map->astar(ghost_tile, pacman_tile);
    } else {
      m_reg_direction = map->astar(ghost_tile, pacman_tile);
    }
    return;
  case Direction::RIGHT:
    if (map->can_go(pacman_tile.i, pacman_tile.j + 3, pacman_dir)) {
      pacman_tile.i = pacman_tile.j + 3;
      m_reg_direction = map->astar(ghost_tile, pacman_tile);
    } else {
      m_reg_direction = map->astar(ghost_tile, pacman_tile);
    }
    return;
  default: m_reg_direction = map->astar(ghost_tile, pacman_tile);
  }
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
                         std::tuple<int, int> pacman_pos,
                         Direction pacman_dir) {
  switch (type) {
  case GhostType::BLINKY: blinky_chase(map, pacman_pos, pacman_dir); break;
  case GhostType::PINKY: break;
  case GhostType::INKY: break;
  case GhostType::CLYDE: break;
  default: break;
  }
}

void Ghost::update(std::shared_ptr<Map> map, std::tuple<int, int> Pacman_pos,
                   Direction pacman_dir) {

  if (is_at_home) {

    // first check if we can still go in the current direction
    if (can_change_direction(map) && can_go(map, opposite(m_direction))) {
      // in that case we simply stop
      m_direction = opposite(m_direction);
    }
  }

  else {
    chase_pacman(map, Pacman_pos,
                 pacman_dir); // update the registered direction

    // if the ghost can no longer go straight
    if (can_change_direction(map) && !can_go(map, m_direction)) {
      m_direction = Direction::NONE;
    }

    // check if we can go in the registered direction
    if (can_change_direction(map) && can_go(map, m_reg_direction)) {
      m_direction = m_reg_direction;
      m_reg_direction = Direction::NONE;
    }
  }
  move();
}

void Ghost::move() {

  switch (m_direction) {
  case Direction::UP: m_cy -= 1; break;
  case Direction::DOWN: m_cy += 1; break;
  case Direction::LEFT: m_cx -= 1; break;
  case Direction::RIGHT: m_cx += 1; break;

  default: break;
  }
}
