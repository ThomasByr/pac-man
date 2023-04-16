
#ifndef __inc_tile_H__
#define __inc_tile_H__

#include <SDL.h>

#include "helper/renderer.h"

/* from map.txt
0 : empty (we could possibly go here)
1 : wall
2 : dot
3 : power dot
4 : ghost house door
5 : portal
9 : start position
*/

enum class TileType {
  EMPTY,
  WALL,
  DOT,
  POWER_DOT,
  GHOST_HOUSE_DOOR,
  PORTAL,
  START_POSITION = 9 // so that we can use static_cast<TileType>(9)
};

class Tile {
private:
  TileType m_type; // type of the tile
  int m_i, m_j;    // position of the tile in the array
  double size;        // size of the tile

public:
  Tile(const int type, const int x, const int y, const double size);

  void show(std::shared_ptr<Renderer> renderer);
};

#endif // __inc_tile_H__
