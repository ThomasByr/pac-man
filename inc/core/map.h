
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

struct NodeHash {
  std::size_t operator()(const Node &n) const {
    return std::hash<int>()(n.i) ^ std::hash<int>()(n.j);
  }
};

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

public:
  Map(double size, const std::string &path = "assets/map.txt");
  void show(std::shared_ptr<Renderer> renderer);

  /// @brief Get the total width of the map
  [[nodiscard]] int get_width() const;
  /// @brief Get the total height of the map
  [[nodiscard]] int get_height() const;

  /// @brief Get the size of one tile
  [[nodiscard]] double get_size() const;

  /// @brief Get the center coordinates of the start tile for pacman
  std::tuple<double, double> get_start_tile_c() const;

  std::tuple<double, double> get_blinky_start_tile_c() const;

  std::tuple<double, double> get_pinky_start_tile_c() const;

  std::tuple<double, double> get_inky_start_tile_c() const;

  std::tuple<double, double> get_clyde_start_tile_c() const;

  bool ate_food(const int i, const int j) const;
  void eat_food(const int i, const int j);

  bool can_go(const int i, const int j, const Direction &dir) const;

  std::vector<Node> get_neighbors(struct Node current) const;
  double heuristic_cost_estimate(struct Node start, struct Node end) const;
  Direction reconstruct_path(std::unordered_map<Node, Node, NodeHash> came_from,
                             Node current) const;
  Direction astar(struct Node start, struct Node end) const;
};

#endif // __inc_core_map_H__
