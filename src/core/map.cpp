
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

int Map::lowest_heuristic(std::vector<std::shared_ptr<Node>> list) const {

  if (list.size() == 0) {
    fmt::panic("lowest_heuristic reading in nothing");
  } else if (list.size() == 1) {
    return list[0]->heuristic;
  }

  int best_value = list[0]->heuristic;
  int best_position = 0;

  int taille = list.size();

  for (int i = 1; i < taille; i++) {
    if (list[i]->heuristic < best_value) {
      best_value = list[i]->heuristic;
      best_position = i;
    }
  }

  return best_position;
}

void Map::find_voisin(std::vector<std::shared_ptr<Node>> voisin,
                      std::shared_ptr<Node> current) const {

  // On récupère la tile du dessus
  Tile upper_tile = m_map[current->x][current->y + 1];
  // On régarde si on peut y accéder : ie ce n'est pas un mur et un portail
  if (upper_tile.get_type() != TileType::WALL &&
      upper_tile.get_type() != TileType::GHOST_HOUSE_DOOR) {
    auto uppervoisin =
      std::make_shared<Node>(Node{current->x, current->y + 1, 0, 0});
    voisin.push_back(uppervoisin);
  }

  Tile lower_tile = m_map[current->x][current->y - 1];
  if (lower_tile.get_type() != TileType::WALL &&
      lower_tile.get_type() != TileType::GHOST_HOUSE_DOOR) {
    auto lowervoisin =
      std::make_shared<Node>(Node{current->x, current->y - 1, 0, 0});
    voisin.push_back(lowervoisin);
  }

  Tile left_tile = m_map[current->x - 1][current->y];
  if (left_tile.get_type() != TileType::WALL &&
      left_tile.get_type() != TileType::GHOST_HOUSE_DOOR) {
    auto leftvoisin =
      std::make_shared<Node>(Node{current->x - 1, current->y, 0, 0});
    voisin.push_back(leftvoisin);
  }

  Tile right_tile = m_map[current->x + 1][current->y];
  if (right_tile.get_type() != TileType::WALL &&
      right_tile.get_type() != TileType::GHOST_HOUSE_DOOR) {
    auto rightvoisin =
      std::make_shared<Node>(Node{current->x + 1, current->y, 0, 0});
    voisin.push_back(rightvoisin);
  }
}

bool Map::find_node(std::vector<std::shared_ptr<Node>> list,
                    std::shared_ptr<Node> current) const {

  int taille = list.size();
  // on regarde si notre noeud est dans la liste
  for (int i = 0; i < taille; i++) {
    if (list[i]->x == current->x && list[i]->y == current->y) return true;
  }

  return false;
}

// Fonction qui regarde si un noeud est présent dans la liste et si son coût est
// inférieur à l'actuel
bool Map::check_open(std::vector<std::shared_ptr<Node>> list,
                     std::shared_ptr<Node> current) const {

  int taille = list.size();
  // on regarde si notre noeud est dans la liste
  for (int i = 0; i < taille; i++) {
    if (list[i]->x == current->x && list[i]->y == current->y) {
      // on regarde si son coût est inférieur à l'actuel
      if (list[i]->cost <= current->cost) { return true; }
    };
  }

  return false;
}

float Map::calc_norm_eucl(std::shared_ptr<Node> start,
                          std::shared_ptr<Node> target) const {

  return sqrt(pow(start->x - target->x, 2) + pow((start->y - target->y), 2));
}

Direction Map::shortest_path(std::shared_ptr<Node> target,
                             std::shared_ptr<Node> starter) const {
  // Liste des noeuds qu'on a déjà exploré
  std::vector<std::shared_ptr<Node>> closedlist{};

  // Liste des noeuds qu'on va explorer
  std::vector<std::shared_ptr<Node>> openlist{};

  openlist.push_back(starter);

  // tant qu'on a des noeuds à explorer
  while (openlist.size() != 0) {

    // on récupère le noeud avec la plus faible heuristique
    int i_want_to_analyze = lowest_heuristic(openlist);
    std::shared_ptr<Node> to_analyze = openlist[i_want_to_analyze];

    // Si on est arrivé
    if (to_analyze->x == target->x && to_analyze->y == target->y) {
      // To do : reconstituer le chemin et récupéré la première direction
      return Direction::NONE;
    }

    // On cherche tous les voisins accessibles de notre tile
    std::vector<std::shared_ptr<Node>> voisin{};
    find_voisin(voisin, to_analyze);
    int taille = voisin.size();
    for (int i = 0; i < taille; i++) {
      if (!(find_node(closedlist, voisin[i]) ||
            check_open(openlist, voisin[i]))) {

        // puis on applique l'algo
        voisin[i]->cost++;
        voisin[i]->heuristic =
          voisin[i]->cost + calc_norm_eucl(voisin[i], target);
        openlist.push_back(voisin[i]);
      }
    }
    closedlist.push_back(to_analyze);
    openlist.erase(openlist.begin() + i_want_to_analyze);
  }
  fmt::panic("A* finding no path");
  return Direction::NONE;
}