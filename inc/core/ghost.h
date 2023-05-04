
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
  bool is_at_home;

public:
  Ghost(const double cx, const double cy, GhostType type, bool is_at_home);
  Ghost(const Ghost &other) = delete;
  Ghost(Ghost &&other) = delete;
  Ghost &operator=(const Ghost &other) = delete;
  Ghost &operator=(Ghost &&other) = delete;

  virtual ~Ghost() = default;

  void show(std::shared_ptr<Renderer> renderer) override;

  bool can_go(std::shared_ptr<Map> map, const Direction &dir) const;

  bool can_change_direction(std::shared_ptr<Map> map) const;

  void update(std::shared_ptr<Map> map, std::tuple<int, int> pacman_pos);

  void chase_pacman(std::shared_ptr<Map> map, std::tuple<int, int> pacman_pos);

  void blinky_chase(std::shared_ptr<Map> map, std::tuple<int, int> pacman_pos);

  void pinky_chase(std::shared_ptr<Map> map, std::tuple<int, int> pacman_pos);

  void inky_chase(std::shared_ptr<Map> map, std::tuple<int, int> pacman_pos);

  void clyde_chase(std::shared_ptr<Map> map, std::tuple<int, int> pacman_pos);

  void move();
};

#endif // __inc_core_ghost_H__
