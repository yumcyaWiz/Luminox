#ifndef HIT_H
#define HIT_H
#include "vec3.h"

class Primitive;

class Hit {
  public:
    float t;
    Vec3 hitPos;
    Vec3 hitNormal;
    const Primitive* hitPrimitive;

    Hit() {};
    Hit(float _t, const Vec3& _hitPos, const Vec3& _hitNormal, const Primitive* _hitPrimitive) : t(_t), hitPos(_hitPos), hitNormal(_hitNormal), hitPrimitive(_hitPrimitive) {};
};
#endif
