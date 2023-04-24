
#ifndef __inc_core_pcmn_H__
#define __inc_core_pcmn_H__

#include <SDL.h>

#include <memory>

#include "helper/renderer.h"

#include "entity.h"

class Pacman : public Entity {
public:
  Pacman(const double cx, const double cy, const double w, const double h);
  Pacman(const Pacman &other) = delete;
  Pacman(Pacman &&other) = delete;
  Pacman &operator=(const Pacman &other) = delete;
  Pacman &operator=(Pacman &&other) = delete;

  virtual ~Pacman() = default;

  void show(std::shared_ptr<Renderer> renderer);

  bool can_go(std::shared_ptr<Map> map, const Direction &dir) const;
  bool can_change_direction(std::shared_ptr<Map> map) const;

  void update(std::shared_ptr<Map> map);
};

#endif // __inc_core_pcmn_H__
