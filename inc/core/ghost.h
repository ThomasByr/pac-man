
#ifndef __inc_core_ghost_H__
#define __inc_core_ghost_H__

#include <SDL2/SDL.h>

#include "entity.h"

class Ghost : public Entity {
public:
  Ghost();
  void show(SDL_Surface *surface);
};

#endif // __inc_core_ghost_H__
