
#ifndef __inc_core_ghost_H__
#define __inc_core_ghost_H__

#include <SDL.h>

#include <memory>

#include "helper/renderer.h"
#include "state.h"

#include "entity.h"

enum class GhostType { BLINKY, PINKY, INKY, CLYDE };
// red, pink, light blue, orange

class Ghost : public Entity {
private:
  GhostType type;
  GhstState state;
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
  void update(std::shared_ptr<Map> map, std::tuple<int, int> pacman_pos,
              Direction pacman_dir);

  void chase_pacman(std::shared_ptr<Map> map, std::tuple<int, int> pacman_pos,
                    Direction pacman_dir);

  void blinky_chase(std::shared_ptr<Map> map, std::tuple<int, int> pacman_pos);
  void pinky_chase(std::shared_ptr<Map> map, std::tuple<int, int> pacman_pos,
                   Direction pacman_dir);
  void inky_chase(std::shared_ptr<Map> map, std::tuple<int, int> pacman_pos,
                  std::tuple<int, int> blinky_pos, Direction pacman_dir);
  void clyde_chase(std::shared_ptr<Map> map, std::tuple<int, int> pacman_pos);
  void scatter(std::shared_ptr<Map> map);
  void frightened(std::shared_ptr<Map> map);
  void eaten(std::shared_ptr<Map> map);

  void move(std::shared_ptr<Map> map);
};

#endif // __inc_core_ghost_H__
