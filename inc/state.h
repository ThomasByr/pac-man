
#ifndef __inc_state_H__
#define __inc_state_H__

#include <iostream>

enum class GameState { MENU, GAME, PAUSE, GAME_OVER, EXIT };
enum class PcmnState { ALIVE, DEAD, POWERED };

std::ostream &operator<<(std::ostream &os, const GameState &dir);
std::string to_string(const GameState &dir);

std::ostream &operator<<(std::ostream &os, const PcmnState &dir);
std::string to_string(const PcmnState &dir);

#endif
