
#ifndef __inc_core_ghost_H__
#define __inc_core_ghost_H__

#include <SDL2/SDL.h>

#include <memory>

#include "helper/renderer.h"

#include "entity.h"

enum class GhostType { BLINKY, PINKY, INKY, CLYDE };

class Ghost : public Entity {
private:
  GhostType type;

public:
  Ghost(const double cx, const double cy, const double w, const double h, GhostType type);
  Ghost(const Ghost &other) = delete;
  void show(std::shared_ptr<Renderer> renderer);
};

#endif // __inc_core_ghost_H__
