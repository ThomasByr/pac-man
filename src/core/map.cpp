
#include <SDL.h>

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "core/map.h"
#include "tile.h"

#include "utils.h"

Map::Map(double size, const std::string &path)
  : m_map{}, size{size}, m_start_tile_cx{0}, m_start_tile_cy{0},
    is_pcmn_powered{false} {

  door_node = {0, 0};

  reset(path);

  if (m_start_tile_cx == 0 && m_start_tile_cy == 0) {
    fmt::panic("Map::Map: pacman no start tile found");
  }
  if (inky_start_tile_cx == 0 && inky_start_tile_cy == 0) {
    fmt::panic("Map::Map: inky no start tile found");
  }
  if (pinky_start_tile_cx == 0 && pinky_start_tile_cy == 0) {
    fmt::panic("Map::Map: pinky no start tile found");
  }
  if (blinky_start_tile_cx == 0 && blinky_start_tile_cy == 0) {
    fmt::panic("Map::Map: blinky no start tile found");
  }
  if (clyde_start_tile_cx == 0 && clyde_start_tile_cy == 0) {
    fmt::panic("Map::Map: clyde no start tile found");
  }
  if (door_node.i == 0 && door_node.j == 0) {
    fmt::panic("Map::Map: no door found");
  }
  if (fruit_node.i == 0 && fruit_node.j == 0) {
    fmt::panic("Map::Map: no fruit found");
  }

  fmt::debug("Map::Map: pacman start tile found at (%f, %f)", m_start_tile_cx,
             m_start_tile_cy);

  fmt::debug("Map::Map: inky start tile found at (%f, %f)", inky_start_tile_cx,
             inky_start_tile_cy);

  fmt::debug("Map::Map: pinky start tile found at (%f, %f)",
             pinky_start_tile_cx, pinky_start_tile_cy);
  fmt::debug("Map::Map: blinky start tile found at (%f, %f)",
             blinky_start_tile_cx, blinky_start_tile_cy);
  fmt::debug("Map::Map: clyde start tile found at (%f, %f)",
             clyde_start_tile_cx, clyde_start_tile_cy);
}

void Map::show(std::shared_ptr<Renderer> renderer) {
  for (auto &row : m_map) {
    for (auto &tile : row) { tile.show(renderer); }
  }
}

int Map::get_width() const { return m_map[0].size() * size; }
int Map::get_height() const { return m_map.size() * size; }
double Map::get_size() const { return size; }
std::tuple<int, int> Map::get_map_size() const {
  return {m_map[0].size(), m_map.size()};
}

std::tuple<double, double> Map::get_start_tile_c() const {
  return {m_start_tile_cx, m_start_tile_cy};
}

std::tuple<double, double> Map::get_blinky_start_tile_c() const {
  return {blinky_start_tile_cx, blinky_start_tile_cy};
}

std::tuple<double, double> Map::get_pinky_start_tile_c() const {
  return {pinky_start_tile_cx, pinky_start_tile_cy};
}

std::tuple<double, double> Map::get_inky_start_tile_c() const {
  return {inky_start_tile_cx, inky_start_tile_cy};
}

std::tuple<double, double> Map::get_clyde_start_tile_c() const {
  return {clyde_start_tile_cx, clyde_start_tile_cy};
}

std::tuple<int, int> Map::get_blinky_pos() const { return blinky_pos; }
std::vector<std::tuple<int, int>> Map::get_ghosts_pos() const {
  // we work with copies here
  std::vector<std::tuple<int, int>> ghosts_pos;
  ghosts_pos.push_back(blinky_pos);
  ghosts_pos.push_back(pinky_pos);
  ghosts_pos.push_back(inky_pos);
  ghosts_pos.push_back(clyde_pos);
  return ghosts_pos;
}
void Map::set_blinky_pos(const int i, const int j) { blinky_pos = {i, j}; }
void Map::set_pinky_pos(const int i, const int j) { pinky_pos = {i, j}; }
void Map::set_inky_pos(const int i, const int j) { inky_pos = {i, j}; }
void Map::set_clyde_pos(const int i, const int j) { clyde_pos = {i, j}; }

std::tuple<int, int> Map::get_pacman_pos() const { return pacman_pos; }
void Map::set_pacman_pos(const int i, const int j) { pacman_pos = {i, j}; }

bool Map::pcmn_powered() const { return is_pcmn_powered; }
void Map::pcmn_powered(const bool powered) { is_pcmn_powered = powered; }

bool Map::can_go(const int i, const int j, const Direction &dir,
                 bool ghost) const {
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
  return ghost ? my_tile.can_go_ghost(target) : my_tile.can_go(target);
}

double Map::distance(const struct Node &from, const struct Node &to) const {
  // same as heuristic_cost_estimate
  return std::sqrt(std::pow(from.i - to.i, 2) + std::pow(from.j - to.j, 2));
}

bool Map::ate_food(const int i, const int j) const {
  return m_map[i][j].get_type() == TileType::DOT;
}
void Map::eat_food(const int i, const int j) {
  if (m_map[i][j].get_type() == TileType::DOT) {
    m_map[i][j].set_type(TileType::EMPTY);
  }
}

bool Map::ate_big_food(const int i, const int j) const {
  return m_map[i][j].get_type() == TileType::POWER_DOT;
}
void Map::eat_big_food(const int i, const int j) {
  if (m_map[i][j].get_type() == TileType::POWER_DOT) {
    m_map[i][j].set_type(TileType::EMPTY);
  }
}

double Map::heuristic_cost_estimate(const struct Node &start,
                                    const struct Node &end) const {
  return std::sqrt(std::pow(start.i - end.i, 2) + std::pow(start.j - end.j, 2));
}

std::vector<Node> Map::get_neighbors(const struct Node &current) const {
  std::vector<Node> neighbours;
  if (can_go(current.i, current.j, Direction::UP, true)) {
    neighbours.push_back({current.i - 1, current.j});
  }
  if (can_go(current.i, current.j, Direction::DOWN, true)) {
    neighbours.push_back({current.i + 1, current.j});
  }
  if (can_go(current.i, current.j, Direction::LEFT, true)) {
    neighbours.push_back({current.i, current.j - 1});
  }
  if (can_go(current.i, current.j, Direction::RIGHT, true)) {
    neighbours.push_back({current.i, current.j + 1});
  }
  return neighbours;
}

Direction Map::reconstruct_path(
  std::unordered_map<Node, Node, NodeHash> came_from, Node current) const {
  std::vector<Node> total_path;
  total_path.push_back(current);
  while (came_from.find(current) != came_from.end()) {
    current = came_from[current];
    total_path.push_back(current);
  }
  total_path.pop_back();
  Node next = total_path.back();
  total_path.pop_back();
  return get_direction_from_nodes(current, next);
}

Direction Map::astar(const struct Node &start, const struct Node &end) const {
  std::unordered_set<Node, NodeHash> closed_set;
  std::unordered_set<Node, NodeHash> open_set;

  open_set.insert(start);

  std::unordered_map<Node, Node, NodeHash> came_from;

  std::unordered_map<Node, double, NodeHash> g_score;
  g_score[start] = 0;

  std::unordered_map<Node, double, NodeHash> f_score;
  f_score[start] = heuristic_cost_estimate(start, end);

  while (!open_set.empty()) {
    Node current = *std::min_element(
      open_set.begin(), open_set.end(),
      [&](const Node &a, const Node &b) { return f_score[a] < f_score[b]; });

    if (current == end) { return reconstruct_path(came_from, current); }

    open_set.erase(current);
    closed_set.insert(current);

    for (auto &neighbor : get_neighbors(current)) {
      if (closed_set.find(neighbor) != closed_set.end()) { continue; }

      double tentative_g_score = g_score[current] + 1;

      if (open_set.find(neighbor) == open_set.end()) {
        open_set.insert(neighbor);
      } else if (tentative_g_score >= g_score[neighbor]) {
        continue;
      }

      came_from[neighbor] = current;
      g_score[neighbor] = tentative_g_score;
      f_score[neighbor] =
        g_score[neighbor] + heuristic_cost_estimate(neighbor, end);
    }
  }
  return Direction::NONE;
}

Direction Map::stupid(const struct Node &start, const struct Node &end,
                      const Direction &current) const {
  std::unordered_map<Node, double, NodeHash> f_score;
  for (auto &neighbor : get_neighbors(start)) {
    f_score[neighbor] = heuristic_cost_estimate(neighbor, end);
  }
  // chose the direction that is the closest to the target
  auto [target, _] = *std::min_element(
    f_score.begin(), f_score.end(),
    [&](const auto &a, const auto &b) { return a.second < b.second; });
  // get the direction to the target
  auto d = get_direction_from_nodes(start, target);
  // if direction is opposite to the current direction, chose the second best
  if (current != Direction::NONE && d == opposite(current) &&
      f_score.size() > 1) {
    f_score.erase(target);
    auto [target, _] = *std::min_element(
      f_score.begin(), f_score.end(),
      [&](const auto &a, const auto &b) { return a.second < b.second; });
    d = get_direction_from_nodes(start, target);
  }

  return d;
}

bool Map::is_home(const int i, const int j) const {
  return i >= 9 && i <= 12 && j >= 12 && j <= 13;
}

struct Node Map::get_door_node() const { return door_node; }

std::shared_ptr<sys_pause::Timer> Map::get_power_timer() { return power_timer; }

void Map::reset(const std::string &path) {
  using namespace std;

  power_timer = std::make_shared<sys_pause::Timer>();

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
      if (c == '4') {
        door_node = {i, j};
      } else if (c == '6') {
        fruit_node = {i, j};
      } else if (c == '9') {
        m_start_tile_cx = static_cast<double>(j) * size + size / 2.0;
        m_start_tile_cy = static_cast<double>(i) * size + size / 2.0;
      } // save the center position of the start tile
      else if (c == 'i') {
        inky_start_tile_cx = static_cast<double>(j) * size + size / 2.0;
        inky_start_tile_cy = static_cast<double>(i) * size + size / 2.0;
      } else if (c == 'p') {
        pinky_start_tile_cx = static_cast<double>(j) * size + size / 2.0;
        pinky_start_tile_cy = static_cast<double>(i) * size + size / 2.0;
      } else if (c == 'c') {
        clyde_start_tile_cx = static_cast<double>(j) * size + size / 2.0;
        clyde_start_tile_cy = static_cast<double>(i) * size + size / 2.0;
      } else if (c == 'b') {
        blinky_start_tile_cx = static_cast<double>(j) * size + size / 2.0;
        blinky_start_tile_cy = static_cast<double>(i) * size + size / 2.0;
      }
      j++;
    }
    m_map.push_back(row);
    i++;
    j = 0;
  }
}

void Map::set_ghosts_powered(const bool powered, std::optional<int> ghost) {
  if (!ghost.has_value()) {
    // set all ghosts to powered
    for (int i = 0; i < 4; i++) { ghosts_powered.at(i) = powered; }
  } else {
    // set only the specified ghost to powered
    ghosts_powered.at(ghost.value()) = powered;
  }
}

int Map::get_ghosts_powered(int ghost) const {
  return ghosts_powered.at(ghost);
}
