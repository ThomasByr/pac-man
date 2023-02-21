
#ifndef __inc_tile_H__
#define __inc_tile_H__

#include <SDL2/SDL.h>

#include "interface/showable.h"

/* from map.txt
0 : empty (we could possibly go here)
1 : wall
2 : dot
3 : power pellet
4 : ghost house door
5 : portal
9 : start position
*/

enum class TileType {
  EMPTY,
  WALL,
  DOT,
  POWER_PELLET,
  GHOST_HOUSE_DOOR,
  PORTAL,
  START_POSITION = 9 // so that we can use static_cast<TileType>(9)
};

class Tile : public Showable {
private:
  TileType m_type; // type of the tile
  int m_x, m_y;    // position of the tile in the array

public:
  Tile(const int type, const int x, const int y);

  void show(SDL_Surface *surface) override;
};

#endif // __inc_tile_H__
