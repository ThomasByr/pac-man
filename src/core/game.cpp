
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
  : m_running{true}, w_sep{0}, m_points_per_dot{0}, m_points_per_power_dot{0},
    m_points_per_ghost{0} {
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
    std::make_shared<Ghost>(cx_blinky, cy_blinky, GhostType::BLINKY);
  m_ghosts[1] = std::make_shared<Ghost>(cx_pinky, cy_pinky, GhostType::PINKY);
  m_ghosts[2] = std::make_shared<Ghost>(cx_inky, cy_inky, GhostType::INKY);
  m_ghosts[3] = std::make_shared<Ghost>(cx_clyde, cy_clyde, GhostType::CLYDE);
}

Game::~Game() = default;

void Game::run() {
  FPSCounter fps_counter;
  Uint64 fps = 0;

  while (m_running) {
    // handle quit event
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) { m_running = false; }

      // handle key events
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {

        case SDLK_UP: m_pacman->set_direction(Direction::UP); break;
        case SDLK_DOWN: m_pacman->set_direction(Direction::DOWN); break;
        case SDLK_LEFT: m_pacman->set_direction(Direction::LEFT); break;
        case SDLK_RIGHT: m_pacman->set_direction(Direction::RIGHT); break;
        default: break;
        }
      }
    }

    // update
    fps = fps_counter.tick();
    m_pacman->update(m_map);

    std::tuple<int, int> pacman_pos = m_pacman->get_ij(m_map->get_size());

    m_ghosts[0]->update(m_map, pacman_pos);
    m_ghosts[1]->update(m_map, pacman_pos);
    m_ghosts[2]->update(m_map, pacman_pos);
    m_ghosts[3]->update(m_map, pacman_pos);

    // render
    m_renderer->clear();
    m_renderer->blit(m_assets->m_bg, 0, 0);
    m_renderer->text(fmt::format("FPS %d", fps), w_sep + 10, 10);
    m_renderer->text(fmt::format("SCORE %d", m_pacman->get_score()), w_sep + 10,
                     30);

    m_map->show(m_renderer);

    // show entities after map & bg
    m_pacman->show(m_renderer);

    m_ghosts[0]->show(m_renderer);
    m_ghosts[1]->show(m_renderer);
    m_ghosts[2]->show(m_renderer);
    m_ghosts[3]->show(m_renderer);

    // flip buffers and wait
    m_renderer->flip();
  }
}
