
#ifndef __inc_core_entity_H__
#define __inc_core_entity_H__

#include <SDL2/SDL.h>

#include "interface/showable.h"

class Entity : public Showable {
private:
  int m_cx, m_cy; // center position of the entity on the map
  int w, h;       // width and height of the entity

public:
  Entity();
  virtual void show(SDL_Surface *surface) = 0;
};

#endif // __inc_core_entity_H__
