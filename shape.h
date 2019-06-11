#ifndef SHAPE_H
#define SHAPE_H
#include <cmath>
#include "ray.h"
#include "hit.h"
#include "sampler.h"


class Shape {
  public:
    virtual bool intersect(const Ray&, Hit& res) const = 0;
    virtual Vec3 sample(const Hit& res, Sampler& sampler, float& pdf_A) const = 0;
};


class Sphere : public Shape {
  public:
    Vec3 center;
    float radius;

    Sphere(const Vec3& _center, float _radius) : center(_center), radius(_radius) {};

    bool intersect(const Ray& ray, Hit& res) const {
      float b = dot(ray.origin - center, ray.direction);
      float c = (ray.origin - center).length2() - radius*radius;
      float D = b*b - c;
      if(D < 0) return false;

      float t1 = -b - std::sqrt(D);
      float t2 = -b + std::sqrt(D);
      float t = t1;
      if(t < ray.tmin || t > ray.tmax) {
        t = t2;
        if(t < ray.tmin || t > ray.tmax) return false;
      }

      res.t = t;
      res.hitPos = ray(t);
      res.hitNormal = normalize(res.hitPos - center);
      return true;
    };

    Vec3 sample(const Hit& res, Sampler& sampler, float& pdf_A) const {
      Vec3 p = sampleUniformHemisphere(sampler.getNext(), sampler.getNext(), pdf_A);
      Vec3 n = normalize(center - res.hitPos);
      Vec3 s, t;
      orthonormalBasis(n, s, t);
      return center + s*p.x + n*p.y + t*p.z;
    };
};


class Plane : public Shape {
  public:
    Vec3 center;
    float width;
    float height;
    Vec3 normal;
    Vec3 right;
    Vec3 forward;

    Plane(const Vec3& _center, float _width, float _height, const Vec3& _normal, const Vec3& _right) : center(_center), width(_width), height(_height), normal(_normal), right(_right) {
      forward = cross(right, normal);
    };

    bool intersect(const Ray& ray, Hit& res) const {
      float t = -dot(ray.origin - center, normal) / dot(ray.direction, normal);
      if(t < ray.tmin || t > ray.tmax) return false;

      Vec3 hitPos = ray(t);
      float dx = dot(hitPos - center, right);
      float dy = dot(hitPos - center, forward);
      if(std::abs(dx) > width/2 || std::abs(dy) > height/2) return false;

      res.t = t;
      res.hitPos = hitPos;
      res.hitNormal = dot(-ray.direction, normal) > 0 ? normal : -normal;
      return true;
    };

    Vec3 sample(const Hit& res, Sampler& sampler, float& pdf_A) const {
      pdf_A = 1 / (width*height);
      float u = 2*sampler.getNext() - 1;
      float v = 2*sampler.getNext() - 1;
      return center + u*right + v*forward;
    };
};
#endif
