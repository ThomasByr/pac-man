
#ifndef __inc_core_pcmn_H__
#define __inc_core_pcmn_H__

#include <SDL2/SDL.h>

#include "entity.h"

class Pacman : public Entity {
public:
  Pacman();
  Pacman(const Pacman &other) = delete;
  void show(SDL_Surface *surface);
};

#endif // __inc_core_pcmn_H__
