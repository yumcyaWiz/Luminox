#ifndef SCENE_H
#define SCENE_H
#include <memory>
#include "ray.h"
#include "hit.h"
#include "primitive.h"
#include "accel.h"
class Scene {
  public:
    Accel primitives;

    Scene() {};

    void add(const std::shared_ptr<Primitive>& prim) {
      primitives.add(prim);
    };

    bool intersect(const Ray& ray, Hit& res) const {
      return primitives.intersect(ray, res);
    };
};
#endif
