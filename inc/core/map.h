
#ifndef __inc_core_map_H__
#define __inc_core_map_H__

#include <SDL2/SDL.h>

#include <string>
#include <vector>

#include "interface/showable.h"

#include "tile.h"

class Map : public Showable {
private:
  std::vector<std::vector<Tile>> m_map;

public:
  Map(const std::string &path = "assets/map.txt");
  void show(SDL_Surface *surface) override;
};

#endif // __inc_core_map_H__
