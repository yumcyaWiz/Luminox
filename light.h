#ifndef LIGHT_H
#define LIGHT_H
#include <memory>
#include "vec3.h"
#include "shape.h"
#include "hit.h"


class Light {
  public:
    Vec3 le;

    Light(const Vec3& _le) : le(_le) {};
    
    virtual Vec3 Le() const = 0;
    virtual Vec3 sample(const Hit& res, Sampler& sampler, Vec3& sampledPos, float& pdf_A) const = 0;
};


class AreaLight : public Light {
  public:
    std::shared_ptr<Shape> shape;

    AreaLight(const Vec3& _le, const std::shared_ptr<Shape>& _shape) : Light(_le), shape(_shape) {};

    Vec3 Le() const {
      return le;
    };

    Vec3 sample(const Hit& res, Sampler& sampler, Vec3& sampledPos, float& pdf_A) const {
      sampledPos = shape->sample(res, sampler, pdf_A);
      return le;
    };
};
#endif
