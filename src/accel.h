#ifndef ACCEL_H
#define ACCEL_H
#include <vector>
#include <memory>
#include "ray.h"
#include "hit.h"
#include "primitive.h"
class Accel {
  public:
    std::vector<std::shared_ptr<Primitive>> primitives;

    Accel() {};

    void add(const std::shared_ptr<Primitive>& primitive) {
      primitives.push_back(primitive);
    };

    bool intersect(const Ray& ray, Hit& res) const {
      bool hit = false;
      res.t = ray.tmax;
      for(auto p : primitives) {
        Hit tmp;
        if(p->intersect(ray, tmp)) {
          if(tmp.t < res.t) {
            hit = true;
            res = tmp;
          }
        }
      }
      return hit;
    };
};
#endif
