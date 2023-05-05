
#ifndef __inc_core_map_H__
#define __inc_core_map_H__

#include <SDL.h>

#include <cmath>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "helper/renderer.h"
#include "tile.h"

struct Node {
  int i, j;
};

inline bool operator==(const Node &lhs, const Node &rhs) {
  return lhs.i == rhs.i && lhs.j == rhs.j;
}
inline bool operator!=(const Node &lhs, const Node &rhs) {
  return !(lhs == rhs);
}
inline Node operator+(const Node &lhs, const Node &rhs) {
  return {lhs.i + rhs.i, lhs.j + rhs.j};
}
inline Node operator-(const Node &lhs, const Node &rhs) {
  return {lhs.i - rhs.i, lhs.j - rhs.j};
}

struct NodeHash {
  std::size_t operator()(const Node &n) const {
    return std::hash<int>()(n.i) ^ std::hash<int>()(n.j);
  }
};

/**
 * @brief get a direction from two adjacent nodes
 *
 * @param from start node
 * @param to   destination node
 * @return Direction
 */
inline Direction get_direction(const Node &from, const Node &to) {
  if (from.i == to.i + 1) { return Direction::UP; }
  if (from.i == to.i - 1) { return Direction::DOWN; }
  if (from.j == to.j + 1) { return Direction::LEFT; }
  if (from.j == to.j - 1) { return Direction::RIGHT; }
  return Direction::NONE;
}

class Map {
private:
  std::vector<std::vector<Tile>> m_map{}; // map of tiles
  double size;                            // size of one tile

  // center position of the start tile for pacman
  double m_start_tile_cx, m_start_tile_cy;

  double blinky_start_tile_cx, blinky_start_tile_cy;

  double pinky_start_tile_cx, pinky_start_tile_cy;

  double inky_start_tile_cx, inky_start_tile_cy;

  double clyde_start_tile_cx, clyde_start_tile_cy;

  std::tuple<int, int> blinky_pos;

public:
  Map(double size, const std::string &path = "assets/map.txt");
  void show(std::shared_ptr<Renderer> renderer);

  /// @brief Get the total width of the map in pixels
  [[nodiscard]] int get_width() const;
  /// @brief Get the total height of the map in pixels
  [[nodiscard]] int get_height() const;

  /// @brief Get the size of one tile
  [[nodiscard]] double get_size() const;

  /// @brief Get the map size in tiles
  [[nodiscard]] std::tuple<int, int> get_map_size() const;

  /// @brief Get the center coordinates of the start tile for pacman
  std::tuple<double, double> get_start_tile_c() const;

  std::tuple<double, double> get_blinky_start_tile_c() const;
  std::tuple<double, double> get_pinky_start_tile_c() const;
  std::tuple<double, double> get_inky_start_tile_c() const;
  std::tuple<double, double> get_clyde_start_tile_c() const;

  std::tuple<int, int> get_blinky_pos() const;
  void set_blinky_pos(const int i, const int j);

  bool ate_food(const int i, const int j) const;
  void eat_food(const int i, const int j);

  bool can_go(const int i, const int j, const Direction &dir) const;
  /// @brief distance between two nodes
  double distance(const struct Node &from, const struct Node &to) const;

  std::vector<Node> get_neighbors(const struct Node &current) const;
  double heuristic_cost_estimate(const struct Node &start,
                                 const struct Node &end) const;
  Direction reconstruct_path(std::unordered_map<Node, Node, NodeHash> came_from,
                             Node current) const;
  Direction astar(const struct Node &start, const struct Node &end) const;

  Direction stupid(const struct Node &start, const struct Node &end,
                   const Direction &current) const;
};

#endif // __inc_core_map_H__
