#ifndef LIGHT_H
#define LIGHT_H
#include "vec3.h"


class Light {
  public:
    Vec3 le;

    Light(const Vec3& _le) : le(_le) {};
};


class AreaLight : public Light {
  public:
    AreaLight(const Vec3& _le) : Light(_le) {};
};
#endif
