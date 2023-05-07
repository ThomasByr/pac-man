
#include <SDL.h>

#include <memory>
#include <tuple>
#include <vector>

#include "ini.hpp"

#include "helper/assets.h"
#include "helper/renderer.h"

#include "core/game.h"
#include "utils.h"

FPSCounter::FPSCounter() = default;
FPSCounter::~FPSCounter() = default;

size_t FPSCounter::tick() {
  Uint64 now = SDL_GetPerformanceCounter();
  Uint64 a_second_ago = now - SDL_GetPerformanceFrequency();
  while (!last_second_frames.empty() &&
         last_second_frames.front() < a_second_ago) {
    last_second_frames.pop_front();
  }
  last_second_frames.push_back(now);
  return last_second_frames.size();
}

Game::Game(const std::string &config_path)
  : m_state{GameState::MENU}, m_running{true}, w_sep{0}, m_points_per_dot{0},
    m_points_per_power_dot{0}, m_points_per_ghost{0} {
  m_renderer = std::make_shared<Renderer>("Pacman", config_path);
  m_assets = m_renderer->get_assets();

  m_map = std::make_shared<Map>(m_renderer->get_size());
  w_sep = m_map->get_width();

  ini::IniFile data;
  try {
    data.load(config_path);
  } catch (const std::exception &e) {
    fmt::panic("Failed to load config file: %s", e.what());
  }

  auto &game = data["game"];
  m_points_per_dot = game["points_per_dot"].as<int>();
  m_points_per_power_dot = game["points_per_power_dot"].as<int>();
  m_points_per_ghost = game["points_per_ghost"].as<int>();

  double cx = 0, cy = 0;
  std::tie(cx, cy) = m_map->get_start_tile_c();
  const struct PacmanConfig c = {m_points_per_dot, m_points_per_power_dot,
                                 m_points_per_ghost};
  m_pacman = std::make_shared<Pacman>(cx, cy, c);

  double cx_pinky = 0, cy_pinky = 0;
  double cx_inky = 0, cy_inky = 0;
  double cx_blinky = 0, cy_blinky = 0;
  double cx_clyde = 0, cy_clyde = 0;
  std::tie(cx_pinky, cy_pinky) = m_map->get_pinky_start_tile_c();
  std::tie(cx_inky, cy_inky) = m_map->get_inky_start_tile_c();
  std::tie(cx_blinky, cy_blinky) = m_map->get_blinky_start_tile_c();
  std::tie(cx_clyde, cy_clyde) = m_map->get_clyde_start_tile_c();

  m_ghosts = std::vector<std::shared_ptr<Ghost>>{4};

  m_ghosts[0] =
    std::make_shared<Ghost>(cx_blinky, cy_blinky, GhostType::BLINKY, false);
  m_ghosts[1] =
    std::make_shared<Ghost>(cx_pinky, cy_pinky, GhostType::PINKY, true);
  m_ghosts[2] =
    std::make_shared<Ghost>(cx_inky, cy_inky, GhostType::INKY, true);
  m_ghosts[3] =
    std::make_shared<Ghost>(cx_clyde, cy_clyde, GhostType::CLYDE, true);
}

Game::~Game() = default;

void Game::run() {
  FPSCounter fps_counter;
  Uint64 fps = 0;

  // pour les allers retours
  m_ghosts[1]->set_direction(Direction::UP);
  m_ghosts[2]->set_direction(Direction::DOWN);
  m_ghosts[3]->set_direction(Direction::DOWN);

  while (m_running) {
    // handle quit event
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) { m_running = false; }

      // handle key events in menu mode
      if (m_state == GameState::MENU && event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {

        case SDLK_RETURN: m_state = GameState::WAITING; break;
        default: break;
        }
      }

      // handle key events in waiting mode
      if (m_state == GameState::WAITING && event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {

        case SDLK_UP:
        case SDLK_DOWN:
        case SDLK_LEFT:
        case SDLK_RIGHT: m_state = GameState::GAME; break;
        default: break;
        }
      }

      // handle key events in game mode
      if (m_state == GameState::GAME && event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {

        case SDLK_UP: m_pacman->set_direction(Direction::UP); break;
        case SDLK_DOWN: m_pacman->set_direction(Direction::DOWN); break;
        case SDLK_LEFT: m_pacman->set_direction(Direction::LEFT); break;
        case SDLK_RIGHT: m_pacman->set_direction(Direction::RIGHT); break;
        default: break;
        }
      }

      // handle left click FOR DEBUG PURPOSES ONLY
      if (m_state == GameState::MENU && event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
          int x, y;
          SDL_GetMouseState(&x, &y);
          fmt::debug("x: %d, y: %d, i: %f, j: %f", x, y, x / m_map->get_size(),
                     y / m_map->get_size());
        }
      }
    }

    // update
    fps = fps_counter.tick();

    // render
    m_renderer->clear();
    m_renderer->blit(m_assets->m_bg, 0, 0);
    m_renderer->text(fmt::format("FPS %d", fps), w_sep + 10, 10);
    m_renderer->text(fmt::format("SCORE %d", m_pacman->get_score()), w_sep + 10,
                     30);

    // render lives
    unsigned lives = m_pacman->get_lives();
    m_renderer->push();
    m_renderer->rect_mode(RectMode::CORNER);
    for (unsigned i = 0; i < lives; ++i) {
      m_renderer->blit(m_assets->m_lives, w_sep + 10 + i * 30,
                       m_map->get_height() -
                         m_assets->m_lives.h * m_renderer->get_scale() - 10);
    }
    m_renderer->pop();

    std::tuple<int, int> pacman_pos = m_pacman->get_ij(m_map->get_size());

    switch (m_state) {

    case GameState::MENU:
      m_renderer->blit(m_assets->m_bg, 0, 0);
      m_renderer->rect_mode(RectMode::CENTER);
      m_renderer->blit(m_assets->m_bg_menu, m_map->get_width() / 2, 200, 0.7);
      m_renderer->rect_mode(RectMode::CORNER);
      if ((2 * m_renderer->get_fps_count() / m_renderer->get_max_fps()) & 1) {
        m_renderer->text("PRESS ENTER TO START", 50, 350);
      }
      break;

    case GameState::GAME:
      m_pacman->update(m_map);

      for (auto &ghost : m_ghosts) {
        ghost->update(m_map, pacman_pos, m_pacman->get_direction());
      }
      /* FALLTHROUGH */
    case GameState::WAITING:
      if (m_state == GameState::WAITING) { /* ready */
        m_renderer->text("READY!", 175, 335);
      }

      m_map->show(m_renderer);

      // show entities after map & bg
      m_pacman->show(m_renderer);
      for (auto &ghost : m_ghosts) { ghost->show(m_renderer); }

      break;

    default: break;
    }

    // flip buffers and wait
    m_renderer->flip();
  }
}
