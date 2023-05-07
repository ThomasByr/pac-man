
#include <SDL.h>

#include "core/ghost.h"

#include "utils.h"

Ghost::Ghost(const double cx, const double cy, GhostType type, bool is_at_home)
  : Entity{cx, cy, 0, 0}, type{type}, state{GhstState::SCATTER},
    is_at_home{is_at_home} {
  if (is_at_home) {
    m_direction = Direction::UP;
  } else {
    m_direction = Direction::NONE;
  }
}

void Ghost::show(std::shared_ptr<Renderer> renderer) {

  SDL_Rect asset;
  static const double custom_scale = 0.6;

  switch (state) {

  case GhstState::FRIGHTENED:
    asset = renderer->get_assets()->get_sprite_ghost_weak(
      renderer->get_fps_anim_count(), p_timer->step_passed(7));
    break;

  case GhstState::EATEN:
    asset = renderer->get_assets()->get_sprite_ghost_eyes(m_direction);
    break;

  default:
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

  if (map->is_home(i, j) || state == GhstState::EATEN) {
    return map->can_go(j, i, dir, true);
  } else {
    return map->can_go(j, i, dir);
  }
}

void Ghost::blinky_chase(std::shared_ptr<Map> map,
                         std::tuple<int, int> pacman_pos) {
  Node pacman_tile = {std::get<1>(pacman_pos), std::get<0>(pacman_pos)};
  auto [i, j] = get_ij(map->get_size());
  Node ghost_tile = {j, i};

  m_reg_direction = map->stupid(ghost_tile, pacman_tile, get_direction());
}

void Ghost::pinky_chase(std::shared_ptr<Map> map,
                        std::tuple<int, int> pacman_pos, Direction pacman_dir) {

  Node pacman_tile = {std::get<1>(pacman_pos), std::get<0>(pacman_pos)};
  auto [i, j] = get_ij(map->get_size());
  Node ghost_tile = {j, i};

  // look 3 tiles ahead of pacman
  switch (pacman_dir) {
  case Direction::UP: pacman_tile.i -= 3; break;
  case Direction::DOWN: pacman_tile.i += 3; break;
  case Direction::LEFT: pacman_tile.j -= 3; break;
  case Direction::RIGHT: pacman_tile.j += 3; break;
  case Direction::NONE: break; // todo: do we really do nothing here ?
  }

  m_reg_direction = map->stupid(ghost_tile, pacman_tile, get_direction());
}

void Ghost::inky_chase(std::shared_ptr<Map> map,
                       std::tuple<int, int> pacman_pos,
                       std::tuple<int, int> blinky_pos, Direction pacman_dir) {
  Node pacman_tile = {std::get<1>(pacman_pos), std::get<0>(pacman_pos)};
  auto [i, j] = get_ij(map->get_size());
  Node ghost_tile = {j, i};

  // look 2 tiles ahead of pacman
  switch (pacman_dir) {
  case Direction::UP: pacman_tile.i -= 2; break;
  case Direction::DOWN: pacman_tile.i += 2; break;
  case Direction::LEFT: pacman_tile.j -= 2; break;
  case Direction::RIGHT: pacman_tile.j += 2; break;
  case Direction::NONE: break; // todo: do we really do nothing here ?
  }

  // get the vector from blinky to pacman
  Node blinky_tile = {std::get<1>(blinky_pos), std::get<0>(blinky_pos)};
  Node blinky_to_pacman = pacman_tile - blinky_tile;

  // get the tile 2 tiles ahead of pacman
  Node target_tile = pacman_tile + blinky_to_pacman;

  m_reg_direction = map->stupid(ghost_tile, target_tile, get_direction());
}

void Ghost::clyde_chase(std::shared_ptr<Map> map,
                        std::tuple<int, int> pacman_pos) {
  Node pacman_tile = {std::get<1>(pacman_pos), std::get<0>(pacman_pos)};
  auto [i, j] = get_ij(map->get_size());
  Node ghost_tile = {j, i};

  // if the distance between pacman and clyde is greater than 8 tiles
  if (map->distance(ghost_tile, pacman_tile) > 8) {
    // clyde behaves like blinky
    blinky_chase(map, pacman_pos);
  } else {
    // clyde goes to the bottom left corner
    Node target_tile = {0, std::get<0>(map->get_map_size()) - 1};
    m_reg_direction = map->stupid(ghost_tile, target_tile, get_direction());
  }
}

void Ghost::chase_pacman(std::shared_ptr<Map> map,
                         std::tuple<int, int> pacman_pos,
                         Direction pacman_dir) {
  switch (type) {
  case GhostType::BLINKY: blinky_chase(map, pacman_pos); break;
  case GhostType::PINKY: pinky_chase(map, pacman_pos, pacman_dir); break;
  case GhostType::INKY:
    inky_chase(map, pacman_pos, map->get_blinky_pos(), pacman_dir);
    break;
  case GhostType::CLYDE: clyde_chase(map, pacman_pos); break;
  default: fmt::unreachable("Invalid ghost type");
  }
}

void Ghost::scatter(std::shared_ptr<Map> map) {
  auto [i, j] = get_ij(map->get_size());
  Node ghost_tile = {j, i};

  Node target = {0, 0};
  switch (type) {
  case GhostType::BLINKY:
    target = {-2, std::get<0>(map->get_map_size()) - 2};
    break;
  case GhostType::PINKY: target = {-2, 2}; break;
  case GhostType::INKY:
    target = {std::get<1>(map->get_map_size()) - 2,
              std::get<0>(map->get_map_size()) - 2};
    break;
  case GhostType::CLYDE:
    target = {std::get<1>(map->get_map_size()) - 2, 2};
    break;
  default: fmt::unreachable("Invalid ghost type");
  }

  m_reg_direction = map->stupid(ghost_tile, target, get_direction());
}

void Ghost::frightened(std::shared_ptr<Map> map) {
  auto [i, j] = get_ij(map->get_size());
  Node ghost_tile = {j, i};

  std::vector<Direction> possible_directions;
  std::vector<Node> possible_nodes = map->get_neighbors(ghost_tile);
  for (auto node : possible_nodes) {
    possible_directions.push_back(get_direction_from_nodes(ghost_tile, node));
  }

  std::uniform_int_distribution<> dist(0, possible_directions.size() - 1);
  int random_index = dist(gen);
  m_reg_direction = possible_directions[random_index];
}

void Ghost::eat(std::shared_ptr<Map> map) {
  state = GhstState::EATEN;
  map->set_ghosts_powered(false, static_cast<int>(type));
}

void Ghost::eaten(std::shared_ptr<Map> map) {
  auto [i, j] = get_ij(map->get_size());
  Node ghost_tile = {j, i};

  /* is at home ?
  -> yes then become normal
  -> no then continue to go home
  */

  Node home_tile = map->get_door_node(); // home coordinate;

  if (ghost_tile.i == home_tile.i && ghost_tile.j == home_tile.j) {
    state = GhstState::CHASE;
  } else {
    m_reg_direction = map->astar(ghost_tile, home_tile);
  }
}

void Ghost::update(std::shared_ptr<Map> map, std::tuple<int, int> pacman_pos,
                   Direction pacman_dir) {

  teleport(map);
  p_timer = map->get_power_timer();

  // start or init the timer the first time
  if (!m_timer.is_running()) {
    if (is_at_home) {
      switch (type) {
      case GhostType::CLYDE: m_timer.start_timer(20); break;
      case GhostType::PINKY: m_timer.start_timer(0); break;
      case GhostType::INKY: m_timer.start_timer(10); break;

      default: m_timer.start_timer(dis(gen));
      }
    } else {
      m_timer.start_timer(7);
    }
  }

  if (!is_at_home && state != GhstState::FRIGHTENED &&
      map->get_ghosts_powered(static_cast<int>(type))) {
    state = GhstState::FRIGHTENED;
    fmt::debug("lol");
    m_timer.reset_timer(); // reset to use global timer
  }

  // change state
  if (m_timer.is_expired() /* || !m_timer.is_running() */) {
    if (is_at_home) {
      is_at_home = false;
      m_timer.reset_timer();
      m_timer.start_timer(7);
      state = GhstState::SCATTER; // not needed ?
    } else {
      switch (state) {
      case GhstState::SCATTER:
        state = GhstState::CHASE;
        m_timer.reset_timer();
        m_timer.start_timer(20);
        break;
      case GhstState::CHASE:
        state = GhstState::SCATTER;
        m_timer.reset_timer();
        m_timer.start_timer(7);
        break;
      // case GhstState::FRIGHTENED:
      //   state = GhstState::SCATTER;
      //   m_timer.reset_timer(); // reset for ghosts that just went out of the
      //                          // house
      //   m_timer.start_timer(7);
      //   break;
      default: break;
      }
    }
  }
  // go out of frightened state
  // we cannot check if the timer is expired because it might be reset
  if (state == GhstState::FRIGHTENED && !p_timer->is_running()) {
    state = GhstState::SCATTER;
    m_timer.reset_timer();
    m_timer.start_timer(7);
  }

  auto [i, j] = get_ij(map->get_size());

  if (is_at_home) {

    // first check if we can still go in the current direction
    if (can_change_direction(map) && can_go(map, opposite(m_direction))) {
      // in that case we simply stop
      m_direction = opposite(m_direction);
    }
  }

  else {
    // update the registered direction
    switch (state) {
    case GhstState::SCATTER: scatter(map); break;
    case GhstState::CHASE: chase_pacman(map, pacman_pos, pacman_dir); break;
    case GhstState::FRIGHTENED: frightened(map); break;
    case GhstState::EATEN: eaten(map); break;
    }
    // override the registered direction if the ghost is still at home
    if (map->is_home(i, j) /* here it is still i, j */) {
      m_reg_direction = map->astar({j, i}, map->get_door_node());
    }

    // if the registered direction is not the opposite of the current direction
    // and the ghost can go in that direction
    if (m_reg_direction != Direction::NONE && can_change_direction(map) &&
        m_direction != opposite(m_reg_direction) &&
        can_go(map, m_reg_direction)) {
      m_direction = m_reg_direction;
      m_reg_direction = Direction::NONE;
    }

    // if the ghost can no longer go straight
    if (can_change_direction(map) && !can_go(map, m_direction)) {
      m_direction = Direction::NONE;
    }
  }
  move(map);
}

void Ghost::move(std::shared_ptr<Map> map) {

  switch (m_direction) {
  case Direction::UP: m_cy -= m_speed; break;
  case Direction::DOWN: m_cy += m_speed; break;
  case Direction::LEFT: m_cx -= m_speed; break;
  case Direction::RIGHT: m_cx += m_speed; break;

  default: break;
  }

  switch (type) {
  case GhostType::BLINKY: map->set_blinky_pos(m_cx, m_cy); break;
  case GhostType::PINKY: map->set_pinky_pos(m_cx, m_cy); break;
  case GhostType::INKY: map->set_inky_pos(m_cx, m_cy); break;
  case GhostType::CLYDE: map->set_clyde_pos(m_cx, m_cy); break;
  default: fmt::unreachable("Invalid ghost type");
  }
}

void Ghost::reset() {
  m_cx = m_start_cx;
  m_cy = m_start_cy;
  is_at_home = type != GhostType::BLINKY;
  m_direction = is_at_home ? Direction::UP : Direction::NONE;
  m_reg_direction = Direction::NONE;
  state = GhstState::SCATTER;
  m_timer.reset_timer(); // timer will start itself in update
}

bool Ghost::is_eaten() const { return state == GhstState::EATEN; }
