
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

#include "utils.h"

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
inline Direction get_direction_from_nodes(const Node &from, const Node &to) {
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

  std::tuple<int, int> blinky_pos, pinky_pos, inky_pos, clyde_pos;
  std::tuple<int, int> pacman_pos;
  struct Node door_node, fruit_node;

  bool is_pcmn_powered;
  std::shared_ptr<sys_pause::Timer> power_timer;
  std::array<bool, 4> ghosts_powered;

public:
  Map(double size, const std::string &path = "assets/map.txt");
  void show(std::shared_ptr<Renderer> renderer);

  /// @brief Get the total width of the map in pixels
  [[nodiscard]] int get_width(void) const;
  /// @brief Get the total height of the map in pixels
  [[nodiscard]] int get_height(void) const;

  /// @brief Get the size of one tile
  [[nodiscard]] double get_size(void) const;

  /// @brief Get the map size in tiles
  [[nodiscard]] std::tuple<int, int> get_map_size(void) const;

  /// @brief Get the center coordinates of the start tile for pacman
  std::tuple<double, double> get_start_tile_c() const;

  std::tuple<double, double> get_blinky_start_tile_c(void) const;
  std::tuple<double, double> get_pinky_start_tile_c(void) const;
  std::tuple<double, double> get_inky_start_tile_c(void) const;
  std::tuple<double, double> get_clyde_start_tile_c(void) const;

  std::tuple<int, int> get_blinky_pos() const;
  std::vector<std::tuple<int, int>> get_ghosts_pos() const;
  void set_blinky_pos(const int i, const int j);
  void set_pinky_pos(const int i, const int j);
  void set_inky_pos(const int i, const int j);
  void set_clyde_pos(const int i, const int j);

  std::tuple<int, int> get_pacman_pos(void) const;
  void set_pacman_pos(const int i, const int j);

  bool pcmn_powered(void) const;
  void pcmn_powered(const bool powered);

  bool ate_food(const int i, const int j) const;
  void eat_food(const int i, const int j);
  bool ate_big_food(const int i, const int j) const;
  void eat_big_food(const int i, const int j);
  bool ate_fruit(const int i, const int j) const;
  void eat_fruit(const int i, const int j);

  bool can_go(const int i, const int j, const Direction &dir,
              bool ghost = false) const;
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

  bool is_home(const int i, const int j) const;
  struct Node get_door_node(void) const;

  std::shared_ptr<sys_pause::Timer> get_power_timer(void);

  void reset(const std::string &path = "assets/map.txt");

  void set_ghosts_powered(const bool powered, std::optional<int> ghost = std::nullopt);
  int get_ghosts_powered(int ghost) const;
};

#endif // __inc_core_map_H__
