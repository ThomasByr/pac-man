
#ifndef __inc_core_map_H__
#define __inc_core_map_H__

#include <SDL2/SDL.h>

#include <string>
#include <vector>
#include <memory>

#include "interface/showable.h"

#include "tile.h"

class Map : public Showable {
private:
  std::vector<std::vector<Tile>> m_map{};
  double size;

public:
  Map(double size, const std::string &path = "assets/map.txt");
  void show(std::shared_ptr<Renderer> renderer) override;

  int get_width(void) const;
  int get_height(void) const;
};

#endif // __inc_core_map_H__
