
#ifndef __inc_core_game_H__
#define __inc_core_game_H__

#include <SDL2/SDL.h>

#include <memory>
#include <vector>

#include "helper/renderer.h"

#include "ghost.h"
#include "map.h"
#include "pcmn.h"

class Game {
private:
  std::unique_ptr<Map> m_map;
  std::unique_ptr<Pacman> m_pacman;
  std::vector<std::unique_ptr<Ghost>> m_ghosts;

  std::unique_ptr<Renderer> m_renderer;

  bool m_running;

public:
  Game(const std::string &config_path = ".ini");
  ~Game();

  void run(void);
};

#endif // __inc_core_game_H__
