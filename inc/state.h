
#ifndef __inc_state_H__
#define __inc_state_H__

#include <iostream>

enum class GameState { MENU, WAITING, PAUSE, GAME, PACMAN_DIE, GAME_OVER, EXIT };
enum class PcmnState { ALIVE, POWERED };
enum class GhstState { CHASE, SCATTER, FRIGHTENED, EATEN };

std::ostream &operator<<(std::ostream &os, const GameState &state);
std::string to_string(const GameState &state);

std::ostream &operator<<(std::ostream &os, const PcmnState &state);
std::string to_string(const PcmnState &state);

std::ostream &operator<<(std::ostream &os, const GhstState &state);
std::string to_string(const GhstState &state);

#endif
