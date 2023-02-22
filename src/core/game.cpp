
#include <SDL2/SDL.h>

#include <memory>
#include <vector>

#include "helper/assets.h"
#include "helper/renderer.h"

#include "core/game.h"

Game::Game(const std::string &config_path) : m_running{true} {
  m_renderer = std::make_shared<Renderer>("Pacman", config_path);
  m_assets = m_renderer->get_assets();

  m_map = std::make_unique<Map>(m_renderer->get_size());
  m_pacman = std::make_unique<Pacman>();
  m_ghosts = std::vector<std::unique_ptr<Ghost>>{4};

  // m_ghosts.push_back(std::make_unique<Ghost>(config_path, GhostType::RED));
  // m_ghosts.push_back(std::make_unique<Ghost>(config_path, GhostType::PINK));
  // m_ghosts.push_back(std::make_unique<Ghost>(config_path, GhostType::BLUE));
  // m_ghosts.push_back(std::make_unique<Ghost>(config_path,
  // GhostType::ORANGE));
}

Game::~Game() = default;

void Game::run() {
  while (m_running) {
    // handle quit event
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) { m_running = false; }
    }

    // update
    m_renderer->clear();
    m_renderer->blit(m_assets->m_bg, 0, 0);

    m_map->show(m_renderer);

    m_renderer->flip();
  }
}
