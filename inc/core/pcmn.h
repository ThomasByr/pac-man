
#ifndef __inc_core_pcmn_H__
#define __inc_core_pcmn_H__

#include <SDL2/SDL.h>

#include "entity.h"

class Pacman : public Entity {
public:
  Pacman();
  void show(SDL_Surface *surface);
};

#endif // __inc_core_pcmn_H__
