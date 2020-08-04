#ifndef SCENE_H
#define SCENE_H
#include <memory>
#include "ray.h"
#include "hit.h"
#include "primitive.h"
#include "accel.h"
#include "light.h"
class Scene {
  public:
    Accel primitives;
    std::vector<std::shared_ptr<Light>> lights;

    Scene() {};

    void add(const std::shared_ptr<Primitive>& prim) {
      primitives.add(prim);
      if(prim->hasLight()) {
        lights.push_back(prim->light);
      }
    };

    void add(const std::shared_ptr<Light>& light) {
      lights.push_back(light);
    }

    bool intersect(const Ray& ray, Hit& res) const {
      return primitives.intersect(ray, res);
    };
};
#endif
