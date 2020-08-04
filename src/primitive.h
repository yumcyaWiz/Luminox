#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include <cmath>
#include <memory>
#include "ray.h"
#include "hit.h"
#include "shape.h"
#include "material.h"
#include "texture.h"


class AreaLight;


class Primitive {
  public:
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
    std::shared_ptr<AreaLight> light;

    Primitive(const std::shared_ptr<Shape>& _shape, const std::shared_ptr<Material>& _material, const std::shared_ptr<AreaLight>& _light) : shape(_shape), material(_material), light(_light) {};

    bool hasLight() const {
      return light != nullptr;
    };

    bool intersect(const Ray& ray, Hit& res) const {
      Hit tmp;
      if(shape->intersect(ray, tmp)) {
        res = tmp;
        res.hitPrimitive = this;
        return true;
      }
      else {
        return false;
      }
    };
};
#endif
