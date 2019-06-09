#ifndef MATERIAL_H
#define MATERIAL_H
#include <memory>
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "texture.h"
#include "sampler.h"


class Material {
  public:
    std::shared_ptr<Texture> texture;

    Material(const std::shared_ptr<Texture>& _tex) : texture(_tex) {};

    virtual Vec3 sample(const Vec3& wo_local) const = 0;
};


class Diffuse : public Material {
  public:
    Diffuse(const std::shared_ptr<Texture>& _tex) : Material(_tex) {};

    Vec3 sample(const Vec3& wo_local, const Hit& res, Sampler& sampler, Vec3& wi_local, float& pdf_w) const {
      float u = sampler.getNext();
      float v = sampler.getNext();
      wi_local = sampleCosineHemisphere(u, v, pdf_w);
      return texture->eval(res);
    };
};
#endif
