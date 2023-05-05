
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
  if (m_start_tile_cx == 0 && m_start_tile_cy == 0) {
    fmt::panic("Map::Map: pacman no start tile found");
  } else if (inky_start_tile_cx == 0 && inky_start_tile_cy == 0) {
    fmt::panic("Map::Map: inky no start tile found");
  } else if (pinky_start_tile_cx == 0 && pinky_start_tile_cy == 0) {
    fmt::panic("Map::Map: pinky no start tile found");
  } else if (blinky_start_tile_cx == 0 && blinky_start_tile_cy == 0) {
    fmt::panic("Map::Map: blinky no start tile found");
  } else if (clyde_start_tile_cx == 0 && clyde_start_tile_cy == 0) {
    fmt::panic("Map::Map: clyde no start tile found");
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

double Map::heuristic_cost_estimate(struct Node start, struct Node end) const {
  return std::sqrt(std::pow(start.i - end.i, 2) + std::pow(start.j - end.j, 2));
}

std::vector<Node> Map::get_neighbors(struct Node current) const {
  std::vector<Node> neighbours;
  if (can_go(current.i, current.j, Direction::UP)) {
    neighbours.push_back({current.i - 1, current.j});
  }
  if (can_go(current.i, current.j, Direction::DOWN)) {
    neighbours.push_back({current.i + 1, current.j});
  }
  if (can_go(current.i, current.j, Direction::LEFT)) {
    neighbours.push_back({current.i, current.j - 1});
  }
  if (can_go(current.i, current.j, Direction::RIGHT)) {
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
  if (next.i == current.i - 1) { return Direction::UP; }
  if (next.i == current.i + 1) { return Direction::DOWN; }
  if (next.j == current.j - 1) { return Direction::LEFT; }
  if (next.j == current.j + 1) { return Direction::RIGHT; }
  return Direction::NONE;
}

Direction Map::astar(struct Node start, struct Node end) const {
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

Direction Map::stupid(struct Node start, struct Node end, Direction cur_dir) {

  int nb_chemins = 0;
  if (can_go(start.i, start.j, Direction::UP)) { nb_chemins++; }
  if (can_go(start.i, start.j, Direction::DOWN)) { nb_chemins++; }
  if (can_go(start.i, start.j, Direction::LEFT)) { nb_chemins++; }
  if (can_go(start.i, start.j, Direction::RIGHT)) { nb_chemins++; }

  if (nb_chemins == 2) {

    fmt::debug("pas calcul");
    if (can_go(start.i, start.j, Direction::UP) &&
        Direction::UP != opposite(cur_dir)) {
      return Direction::UP;
    }
    if (can_go(start.i, start.j, Direction::DOWN) &&
        Direction::DOWN != opposite(cur_dir)) {
      return Direction::DOWN;
    }
    if (can_go(start.i, start.j, Direction::LEFT) &&
        Direction::LEFT != opposite(cur_dir)) {
      return Direction::LEFT;
    }
    if (can_go(start.i, start.j, Direction::RIGHT) &&
        Direction::RIGHT != opposite(cur_dir)) {
      return Direction::RIGHT;
    }
  } else {

    float closest = 15000;
    float calc;
    Direction selected;

    fmt::debug("calcul !");

    if (can_go(start.i, start.j, Direction::UP) &&
        Direction::UP != opposite(cur_dir)) {
      calc =
        sqrt(std::pow(start.i - end.i, 2) + std::pow(start.j - 1 - end.j, 2));
      fmt::debug("up %f", calc);
      if (calc < closest) {
        closest = calc;
        selected = Direction::UP;
      }
    }
    if (can_go(start.i, start.j, Direction::DOWN) &&
        Direction::DOWN != opposite(cur_dir)) {
      calc =
        sqrt(std::pow(start.i - end.i, 2) + std::pow(start.j + 1 - end.j, 2));
      fmt::debug("down %f", calc);
      if (calc < closest) {
        closest = calc;
        selected = Direction::DOWN;
      }
    }
    if (can_go(start.i, start.j, Direction::LEFT) &&
        Direction::LEFT != opposite(cur_dir)) {
      calc =
        sqrt(std::pow(start.i - 1 - end.i, 2) + std::pow(start.j - end.j, 2));
      fmt::debug("left %f", calc);
      if (calc < closest) {
        closest = calc;
        selected = Direction::LEFT;
      }
    }
    if (can_go(start.i, start.j, Direction::RIGHT) &&
        Direction::RIGHT != opposite(cur_dir)) {
      calc =
        sqrt(std::pow(start.i + 1 - end.i, 2) + std::pow(start.j - end.j, 2));
      fmt::debug("right %f", calc);
      if (calc < closest) {
        closest = calc;
        selected = Direction::RIGHT;
      }
    }

    return selected;
  }

  fmt::debug("should not arrive there");
  return cur_dir;
}
