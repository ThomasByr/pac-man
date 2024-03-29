
#ifndef __inc_tile_H__
#define __inc_tile_H__

#include <SDL.h>
#include <optional>

#include "directions.h"
#include "helper/renderer.h"

/* from map.txt
0 : empty (we could possibly go here)
1 : wall
2 : dot
3 : power dot
4 : ghost house door
5 : portal
6 : bonus
9 : start position
*/

enum class TileType {
  EMPTY,
  WALL,
  DOT,
  POWER_DOT,
  GHOST_HOUSE_DOOR,
  PORTAL,
  BONUS,
  START_POSITION = 9 // so that we can use static_cast<TileType>(9)
};

class Tile {
private:
  TileType m_type; // type of the tile
  int m_i, m_j;    // position of the tile in the array
  double size;     // size of the tile

public:
  Tile() = default;
  Tile(const int type, const int x, const int y, const double size);
  ~Tile() = default;

  /// @brief the type of the tile
  TileType get_type(void) const;
  /// @brief set the type of the tile
  void set_type(const TileType &type);

  /**
   * @brief show the tile on the screen
   *
   * @param renderer the renderer to use
   */
  void show(std::shared_ptr<Renderer> renderer);

  /// @brief can we go to the target tile (as pacman)?
  bool can_go(const std::optional<Tile> &target) const;
  /// @brief can we go to the target tile (as a ghost who can go through doors)?
  bool can_go_ghost(const std::optional<Tile> &target) const;
};

#endif // __inc_tile_H__
