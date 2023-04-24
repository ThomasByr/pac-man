
#ifndef __inc_core_map_H__
#define __inc_core_map_H__

#include <SDL.h>

#include <memory>
#include <string>
#include <vector>

#include "helper/renderer.h"
#include "tile.h"

class Map {
private:
  std::vector<std::vector<Tile>> m_map{}; // map of tiles
  double size;                            // size of one tile

public:
  Map(double size, const std::string &path = "assets/map.txt");
  void show(std::shared_ptr<Renderer> renderer);

  /// @brief Get the total width of the map
  [[nodiscard]] int get_width() const;
  /// @brief Get the total height of the map
  [[nodiscard]] int get_height() const;

  /// @brief Get the size of one tile
  [[nodiscard]] double get_size() const;

  bool can_go(const int i, const int j, const Direction &dir) const;
};

#endif // __inc_core_map_H__
