
#ifndef __inc_interface_showable_H__
#define __inc_interface_showable_H__

#include <SDL2/SDL.h>

#include <memory>

#include "helper/renderer.h"

class Showable {
public:
  virtual void show(std::shared_ptr<Renderer> renderer) = 0;
};

#endif // __inc_interface_showable_H__
