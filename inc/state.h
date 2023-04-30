
#ifndef __inc_state_H__
#define __inc_state_H__

#include <iostream>

enum class GameState { MENU, GAME, PAUSE, GAME_OVER, EXIT };
enum class PcmnState { ALIVE, DEAD, POWERED };
enum class GhstState { CHASE, SCATTER, FRIGHTENED };

std::ostream &operator<<(std::ostream &os, const GameState &state);
std::string to_string(const GameState &state);

std::ostream &operator<<(std::ostream &os, const PcmnState &state);
std::string to_string(const PcmnState &state);

#endif
