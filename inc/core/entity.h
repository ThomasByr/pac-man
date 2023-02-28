
#ifndef __inc_core_entity_H__
#define __inc_core_entity_H__

#include <SDL2/SDL.h>

#include <memory>

#include "interface/showable.h"

#include "helper/renderer.h"

class Entity : public Showable {
private:
  double m_cx, m_cy; // center position of the entity on the map
  double w, h;       // width and height of the entity

public:
  Entity(const double cx, const double cy, const double w, const double h);
  virtual void show(std::shared_ptr<Renderer> renderer) = 0;
};

#endif // __inc_core_entity_H__
