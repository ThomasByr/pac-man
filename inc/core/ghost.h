
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

  /**
   * @brief show ghost on the screen
   *
   * @param renderer renderer to use
   */
  void show(std::shared_ptr<Renderer> renderer) override;

  /**
   * @brief return true if ghost can go in the given direction
   *
   * @param map map to use
   * @param dir direction to test
   * @return true  ghost can go in the given direction
   * @return false otherwise
   */
  bool can_go(std::shared_ptr<Map> map, const Direction &dir) const override;
  /// @brief update main ghost state
  void update(std::shared_ptr<Map> map, std::tuple<int, int> pacman_pos,
              Direction pacman_dir);

  /**
   * @brief chase pacman
   *
   * @param map        map to use
   * @param pacman_pos pacman position
   * @param pacman_dir pacman direction
   */
  void chase_pacman(std::shared_ptr<Map> map, std::tuple<int, int> pacman_pos,
                    Direction pacman_dir);

  /// @brief blinky chase algorithm
  void blinky_chase(std::shared_ptr<Map> map, std::tuple<int, int> pacman_pos);
  /// @brief pinky chase algorithm
  void pinky_chase(std::shared_ptr<Map> map, std::tuple<int, int> pacman_pos,
                   Direction pacman_dir);
  /// @brief inky chase algorithm
  void inky_chase(std::shared_ptr<Map> map, std::tuple<int, int> pacman_pos,
                  std::tuple<int, int> blinky_pos, Direction pacman_dir);
  /// @brief clyde chase algorithm
  void clyde_chase(std::shared_ptr<Map> map, std::tuple<int, int> pacman_pos);

  /// @brief everyone's scatter algorithm
  void scatter(std::shared_ptr<Map> map);
  /// @brief everyone's frightened algorithm
  void frightened(std::shared_ptr<Map> map);
  /// @brief everyone's eaten algorithm
  void eaten(std::shared_ptr<Map> map);

  /// @brief move ghost
  void move(std::shared_ptr<Map> map) override;

  /**
   * @brief reset ghost to its initial state
   */
  void reset(void);
  /// @brief return true if ghost is eaten
  bool is_eaten(void) const;
  /// @brief make a ghost eaten
  void eat(std::shared_ptr<Map> map);
};

#endif // __inc_core_ghost_H__
