
#ifndef __inc_core_game_H__
#define __inc_core_game_H__

#include <SDL2/SDL.h>

#include <memory>
#include <vector>

#include "ghost.h"
#include "map.h"
#include "pcmn.h"

class Game {
private:
  Map m_map;
  std::unique_ptr<Pacman> m_pacman;
  std::vector<std::unique_ptr<Ghost>> m_ghosts;
};

#endif // __inc_core_game_H__
