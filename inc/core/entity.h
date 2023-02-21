
#ifndef __inc_core_entity_H__
#define __inc_core_entity_H__

#include <SDL2/SDL.h>

#include "interface/showable.h"

class Entity : public Showable {
private:
  int m_x, m_y; // position of the entity on the map

public:
  Entity();
  virtual void show(SDL_Surface *surface) = 0;
};

#endif // __inc_core_entity_H__
