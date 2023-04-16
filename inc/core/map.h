
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
  std::vector<std::vector<Tile>> m_map{};
  double size;

public:
  Map(double size, const std::string &path = "assets/map.txt");
  void show(std::shared_ptr<Renderer> renderer);

  [[nodiscard]] int get_width() const;
  [[nodiscard]] int get_height() const;
};

#endif // __inc_core_map_H__
