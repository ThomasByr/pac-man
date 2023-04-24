
#include <SDL.h>

#include <memory>
#include <vector>

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

Game::Game(const std::string &config_path) : m_running{true}, w_sep{0} {
  m_renderer = std::make_shared<Renderer>("Pacman", config_path);
  m_assets = m_renderer->get_assets();

  m_map = std::make_shared<Map>(m_renderer->get_size());
  m_pacman = std::make_shared<Pacman>(0, 0, 0, 0);
  m_ghosts = std::vector<std::shared_ptr<Ghost>>{4};

  w_sep = m_map->get_width();

  // todo: make ghosts
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

    // render
    m_renderer->clear();
    m_renderer->blit(m_assets->m_bg, 0, 0);
    m_renderer->text(fmt::format("FPS %d", fps), w_sep, 0);

    m_map->show(m_renderer);
    m_renderer->flip();
  }
}
