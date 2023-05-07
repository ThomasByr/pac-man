
#include <SDL.h>

#include "core/pcmn.h"

#include "utils.h"

Pacman::Pacman(const double cx, const double cy,
               const struct PacmanConfig &config)
  : Entity{cx, cy, 0, 0}, m_points_per_dot{config.m_points_per_dot},
    m_points_per_power_dot{config.m_points_per_power_dot},
    m_points_per_ghost{config.m_points_per_ghost},
    max_number_of_dots{config.max_number_of_dots}, m_dots_eaten{0},
    state{PcmnState::ALIVE} {
  m_speed = 1.05;
}

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

bool Pacman::play_dead(std::shared_ptr<Renderer> renderer) const {
  static int fc = 0;
  static const double custom_scale = 0.5;
  int end = 0;
  const SDL_Rect asset = renderer->get_assets()->get_sprite_pacman_dead(
    fc / renderer->get_update_interval(), end);
  fc += 1;

  renderer->push();
  renderer->rect_mode(RectMode::CENTER);
  renderer->translate(m_cx, m_cy);
  renderer->blit(asset, 0, 0, custom_scale);
  renderer->pop();

  bool ended = fc / renderer->get_update_interval() > end;
  if (ended) { fc = 0; }
  return ended;
}

size_t Pacman::get_score() const { return m_score; }

bool Pacman::can_go(std::shared_ptr<Map> map, const Direction &dir) const {
  int i = -1, j = -1;
  std::tie(i, j) = get_ij(map->get_size());
  if (i == -1 || j == -1) { return false; }

  return map->can_go(j, i, dir); // why j, i and not i, j ?
                                 // well ...
}

bool Pacman::ate_food(std::shared_ptr<Map> map) {
  auto [i, j] = get_ij(map->get_size());
  return map->ate_food(j, i); // why j, i and not i, j ? well ...
}
void Pacman::eat_food(std::shared_ptr<Map> map) {
  auto [i, j] = get_ij(map->get_size());
  map->eat_food(j, i); // those damn vectors

  m_score += m_points_per_dot;
  m_dots_eaten += 1;
}

bool Pacman::ate_big_food(std::shared_ptr<Map> map) {
  auto [i, j] = get_ij(map->get_size());
  return map->ate_big_food(j, i);
}
void Pacman::eat_big_food(std::shared_ptr<Map> map) {
  auto [i, j] = get_ij(map->get_size());
  map->eat_big_food(j, i);

  m_score += m_points_per_power_dot;
  m_dots_eaten += 1;
  state = PcmnState::POWERED;

  p_timer->reset_timer();
  p_timer->start_timer(10);
  map->set_ghosts_powered(true);
}

void Pacman::update(std::shared_ptr<Map> map) {

  teleport(map);
  p_timer = map->get_power_timer();

  // here we test if the timer is running before because, as opposed to ghosts
  // pacman behavior is not always being timed
  if (p_timer->is_running() && p_timer->is_expired()) {
    switch (state) {
    case PcmnState::POWERED:
      state = PcmnState::ALIVE;
      p_timer->reset_timer();
      map->set_ghosts_powered(false);
      break;
    default: break;
    }
  }

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
  move(map);
  if (ate_food(map)) { eat_food(map); }
  if (ate_big_food(map)) { eat_big_food(map); }
}

void Pacman::move(std::shared_ptr<Map> map) {
  switch (m_direction) {
  case Direction::UP: m_cy -= m_speed; break;
  case Direction::DOWN: m_cy += m_speed; break;
  case Direction::LEFT: m_cx -= m_speed; break;
  case Direction::RIGHT: m_cx += m_speed; break;
  default: break;
  }
  map->set_pacman_pos(m_cx, m_cy);
  map->pcmn_powered(state == PcmnState::POWERED);
}

void Pacman::eat_ghost() {
  switch (state) {
  case PcmnState::POWERED: m_score += m_points_per_ghost; break;
  default: fmt::unreachable("Pacman::eat_entity : should call this function "
                            "only when pacman is powered");
  }
}

bool Pacman::ate_all_dots() const { return m_dots_eaten >= max_number_of_dots; }

void Pacman::reset(bool g_o, bool e_l) {
  m_cx = m_start_cx;
  m_cy = m_start_cy;
  m_direction = Direction::NONE;
  m_reg_direction = Direction::NONE;
  state = PcmnState::ALIVE;
  if (e_l) { m_dots_eaten = 0; }
  if (g_o) { m_lives = m_max_lives; }
}

bool Pacman::is_powered() const { return state == PcmnState::POWERED; }
