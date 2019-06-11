#ifndef MATERIAL_H
#define MATERIAL_H
#include <cmath>
#include <memory>
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "texture.h"
#include "sampler.h"


inline bool isEntering(const Vec3& w) {
  return w.y > 0;
}


inline float cosTheta(const Vec3& w) {
  return w.y;
}
inline float absCosTheta(const Vec3& w) {
  return std::abs(w.y);
}


inline float sinTheta(const Vec3& w) {
  float cos = cosTheta(w);
  return std::sqrt(std::max(1 - cos*cos, 0.0f));
}


inline Vec3 reflect(const Vec3& v, const Vec3& n) {
  return -v + 2*dot(v, n)*n;
}


inline float fresnel(const Vec3& v, const Vec3& n, float n1, float n2) {
  float f0 = std::pow((n1 - n2)/(n1 + n2), 2.0);
  float cos = absCosTheta(v);
  return f0 + (1 - f0)*std::pow(std::max(1 - cos, 0.0f), 5.0);
}


inline bool refract(const Vec3& v, const Vec3& n, float n1, float n2, Vec3& r) {
  float cos = absCosTheta(v);
  float sin = sinTheta(v);
  float alpha = n1/n2 * sin;
  if(alpha*alpha > 1.0f) return false;
  r = n1/n2 * (-v + dot(v, n)*n) - std::sqrt(std::max(1 - alpha*alpha, 0.0f))*n;
  return true;
}



class Material {
  public:
    std::shared_ptr<Texture> texture;

    Material(const std::shared_ptr<Texture>& _tex) : texture(_tex) {};

    virtual Vec3 sample(const Vec3& wo_local, const Hit& res, Sampler& sampler, Vec3& wi_local, float& pdf_w) const = 0;
    virtual Vec3 BRDF(const Hit& res, const Vec3& wo_local, const Vec3& wi_local) const = 0;
};


class Diffuse : public Material {
  public:
    Diffuse(const std::shared_ptr<Texture>& _tex) : Material(_tex) {};

    Vec3 sample(const Vec3& wo_local, const Hit& res, Sampler& sampler, Vec3& wi_local, float& pdf_w) const {
      float u = sampler.getNext();
      float v = sampler.getNext();
      wi_local = sampleCosineHemisphere(u, v, pdf_w);
      return texture->eval(res) / M_PI;
    };

    Vec3 BRDF(const Hit& res, const Vec3& wo_local, const Vec3& wi_local) const {
      return texture->eval(res) / M_PI;
    };
};


class Mirror : public Material {
  public:
    Mirror(const std::shared_ptr<Texture>& _tex) : Material(_tex) {};

    Vec3 sample(const Vec3& wo_local, const Hit& res, Sampler& sampler, Vec3& wi_local, float& pdf_w) const {
      wi_local = reflect(wo_local, Vec3(0, 1, 0));
      pdf_w = 1;
      return texture->eval(res) / absCosTheta(wi_local);
    };

    Vec3 BRDF(const Hit& res, const Vec3& wo_local, const Vec3& wi_local) const {
      return Vec3(0);
    };
};


class Glass : public Material {
  public:
    float ior;

    Glass(const std::shared_ptr<Texture>& _tex, float _ior) : Material(_tex), ior(_ior) {};

    Vec3 sample(const Vec3& wo_local, const Hit& res, Sampler& sampler, Vec3& wi_local, float& pdf_w) const {
      float n1 = 1.0;
      float n2 = ior;
      Vec3 normal = Vec3(0, 1, 0);
      if(!isEntering(wo_local)) {
        n1 = ior;
        n2 = 1.0;
        normal = Vec3(0, -1, 0);
      }

      float fr = fresnel(wo_local, normal, n1, n2);
      if(sampler.getNext() < fr) {
        wi_local = reflect(wo_local, normal);
        pdf_w = fr;
        return fr * texture->eval(res) / absCosTheta(wi_local);
      }
      else {
        if(refract(wo_local, normal, n1, n2, wi_local)) {
          pdf_w = 1 - fr;
          return std::pow(n1/n2, 2.0) * (1 - fr) * texture->eval(res) / absCosTheta(wi_local);
        }
        else {
          wi_local = reflect(wo_local, normal);
          pdf_w = 1 - fr;
          return (1 - fr) * texture->eval(res) / absCosTheta(wi_local);
        }
      }
    };

    Vec3 BRDF(const Hit& res, const Vec3& wo_local, const Vec3& wi_local) const {
      return Vec3(0);
    };
};
#endif
