
#ifndef __inc_core_ghost_H__
#define __inc_core_ghost_H__

#include <SDL.h>

#include <memory>

#include "helper/renderer.h"

#include "entity.h"

enum class GhostType { BLINKY, PINKY, INKY, CLYDE };
// red, pink, light blue, orange

class Ghost : public Entity {
private:
  GhostType type;

public:
  Ghost(const double cx, const double cy, GhostType type);
  Ghost(const Ghost &other) = delete;
  Ghost(Ghost &&other) = delete;
  Ghost &operator=(const Ghost &other) = delete;
  Ghost &operator=(Ghost &&other) = delete;

  virtual ~Ghost() = default;

  void show(std::shared_ptr<Renderer> renderer) override;

  bool can_go(std::shared_ptr<Map> map, const Direction &dir) const;

  bool can_change_direction(std::shared_ptr<Map> map) const;

  void update(std::shared_ptr<Map> map, std::tuple<int, int> Pacman_pos);

  void chase_pacman(std::tuple<int, int> Pacman_pos);

  void blinky_chase(std::tuple<int, int> Pacman_pos);

  void pinky_chase(std::tuple<int, int> Pacman_pos);

  void inky_chase(std::tuple<int, int> Pacman_pos);

  void clyde_chase(std::tuple<int, int> Pacman_pos);
};

#endif // __inc_core_ghost_H__
