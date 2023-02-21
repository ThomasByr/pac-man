
#ifndef __inc_interface_showable_H__
#define __inc_interface_showable_H__

#include <SDL2/SDL.h>

class Showable {
public:
  virtual void show(SDL_Surface *surface) = 0;
};

#endif // __inc_interface_showable_H__
