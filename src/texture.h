#ifndef TEXTURE_H
#define TEXTURE_H
#include <cmath>
#include "vec3.h"


class Texture {
  public:
    virtual Vec3 eval(const Hit& res) const = 0;
};


class Mat : public Texture {
  public:
    Vec3 color;

    Mat(const Vec3& _color) : color(_color) {};

    Vec3 eval(const Hit& res) const {
      return color;
    };
};


class Checkerboard : public Texture {
  public:
    Vec3 color1;
    Vec3 color2;
    float frequency;

    Checkerboard(const Vec3& _color1, const Vec3& _color2, float _frequency) : color1(_color1), color2(_color2), frequency(_frequency) {};

    Vec3 eval(const Hit& res) const {
      float v = std::sin(res.hitPos.x*frequency) * std::sin(res.hitPos.y*frequency) * std::sin(res.hitPos.z*frequency);
      if(v < 0) {
        return color1;
      }
      else {
        return color2;
      }
    };
};
#endif
