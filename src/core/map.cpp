
#include <SDL.h>

#include <cctype>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "core/map.h"
#include "tile.h"

#include "utils.h"

Map::Map(double size, const std::string &path)
  : m_map{}, size{size}, m_start_tile_cx{0}, m_start_tile_cy{0} {
  using namespace std;

  ifstream file{path};
  string line = "";

  // pass until we find the <!-- .* --> tag
  while (getline(file, line)) {
    if (line.find("<!--") != string::npos) { break; }
  }

  // read the map (each line is a row until the end of the file)
  int i = 0, j = 0;
  while (getline(file, line)) {
    if (line.find("<!--") != string::npos) { continue; }
    try {
      if (isspace(line.at(0))) { continue; }
    } catch (const out_of_range &e) { continue; }

    vector<Tile> row{};
    for (char c : line) {
      row.push_back(Tile{static_cast<int>(c - '0'), i, j, size});
      if (c == '9') {
        m_start_tile_cx = static_cast<double>(j) * size + size / 2.0;
        m_start_tile_cy = static_cast<double>(i) * size + size / 2.0;
      } // save the center position of the start tile
      j++;
    }
    m_map.push_back(row);
    i++;
    j = 0;
  }
  if (m_start_tile_cx == 0 && m_start_tile_cy == 0) {
    fmt::panic("Map::Map: no start tile found");
  }
  fmt::debug("Map::Map: start tile found at (%f, %f)", m_start_tile_cx,
             m_start_tile_cy);
}

void Map::show(std::shared_ptr<Renderer> renderer) {
  for (auto &row : m_map) {
    for (auto &tile : row) { tile.show(renderer); }
  }
}

int Map::get_width() const { return m_map[0].size() * size; }
int Map::get_height() const { return m_map.size() * size; }
double Map::get_size() const { return size; }

std::tuple<double, double> Map::get_start_tile_c() const {
  return {m_start_tile_cx, m_start_tile_cy};
}

bool Map::can_go(const int i, const int j, const Direction &dir) const {
  Tile my_tile = m_map[i][j];
  std::optional<Tile> target = std::nullopt;
  switch (dir) {
  case Direction::UP:
    if (i > 0) { target = m_map[i - 1][j]; }
    break;
  case Direction::DOWN:
    if (i < static_cast<int>(m_map.size() - 1)) { target = m_map[i + 1][j]; }
    break;
  case Direction::LEFT:
    if (j > 0) { target = m_map[i][j - 1]; }
    break;
  case Direction::RIGHT:
    if (j < static_cast<int>(m_map[0].size() - 1)) { target = m_map[i][j + 1]; }
    break;
  case Direction::NONE: break;
  }
  return my_tile.can_go(target);
}

bool Map::ate_food(const int i, const int j) const {
  return m_map[i][j].get_type() == TileType::DOT;
}

void Map::eat_food(const int i, const int j) {
  if (m_map[i][j].get_type() == TileType::DOT) {
    m_map[i][j].set_type(TileType::EMPTY);
  }
}
