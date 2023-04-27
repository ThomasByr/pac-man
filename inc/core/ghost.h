
#ifndef __inc_core_ghost_H__
#define __inc_core_ghost_H__

#include <SDL.h>

#include <memory>

#include "helper/renderer.h"

#include "entity.h"

enum class GhostType { BLINKY, PINKY, INKY, CLYDE };

class Ghost : public Entity {
private:
  GhostType type;

public:
  Ghost(const double cx, const double cy, const double w, const double h,
        GhostType type);
  Ghost(const Ghost &other) = delete;
  Ghost(Ghost &&other) = delete;
  Ghost &operator=(const Ghost &other) = delete;
  Ghost &operator=(Ghost &&other) = delete;

  virtual ~Ghost() = default;

  void show(std::shared_ptr<Renderer> renderer) override;
};

#endif // __inc_core_ghost_H__
