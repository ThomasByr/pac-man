
#ifndef __inc_core_ghost_H__
#define __inc_core_ghost_H__

#include <SDL2/SDL.h>

#include <memory>

#include "helper/renderer.h"

#include "entity.h"

class Ghost : public Entity {
public:
  Ghost();
  Ghost(const Ghost &other) = delete;
  void show(std::shared_ptr<Renderer> renderer);
};

#endif // __inc_core_ghost_H__
