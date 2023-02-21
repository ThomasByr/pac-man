
#ifndef __inc_core_pcmn_H__
#define __inc_core_pcmn_H__

#include <SDL2/SDL.h>

#include <memory>

#include "helper/renderer.h"

#include "entity.h"

class Pacman : public Entity {
public:
  Pacman();
  Pacman(const Pacman &other) = delete;
  void show(std::shared_ptr<Renderer> renderer);
};

#endif // __inc_core_pcmn_H__
