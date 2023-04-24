
#ifndef __inc_core_game_H__
#define __inc_core_game_H__

#include <SDL.h>

#include <deque>
#include <memory>
#include <vector>

#include "helper/renderer.h"

#include "ghost.h"
#include "map.h"
#include "pcmn.h"

class FPSCounter {
private:
  std::deque<Uint64> last_second_frames{};

public:
  FPSCounter();
  ~FPSCounter();
  size_t tick(void);
};

class Game {
private:
  std::shared_ptr<Map> m_map{};
  std::shared_ptr<Pacman> m_pacman{};
  std::vector<std::shared_ptr<Ghost>> m_ghosts{};

  std::shared_ptr<Renderer> m_renderer;
  std::shared_ptr<Assets> m_assets;

  bool m_running;
  int w_sep;

public:
  Game(const std::string &config_path = ".ini");
  ~Game();

  void run(void);
};

#endif // __inc_core_game_H__
