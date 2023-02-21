
#include <SDL2/SDL.h>

#include <fstream>
#include <iostream>
#include <string>

#include "core/map.h"
#include "tile.h"

Map::Map(const std::string &path) {
  using namespace std;

  ifstream file{path};
  string line;

  // pass until we find the <!-- .* --> tag
  while (getline(file, line)) {
    if (line.find("<!--") != string::npos) { break; }
  }

  // read the map (each line is a row until the end of the file)
  int i = 0, j = 0;
  while (getline(file, line)) {
    if (line.find("<!--") != string::npos) { continue; }

    vector<Tile> row;
    for (char c : line) {
      row.push_back(Tile{static_cast<int>(c), i, j});
      j++;
    }
    m_map.push_back(row);
    i++;
  }
}

void Map::show(SDL_Surface *surface) {
  for (auto &row : m_map) {
    for (auto &tile : row) {
      tile.show(surface);
    }
  }
}
