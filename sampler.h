#ifndef SAMPLER_H
#define SAMPLER_H
#include <cmath>
#include <random>
class Sampler {
  public:
    virtual float getNext() = 0;
};


class UniformSampler : public Sampler {
  public:
    std::random_device rnd_dev;
    std::mt19937 mt;
    std::uniform_real_distribution<float> dist;

    UniformSampler() {
      mt.seed(rnd_dev());
      dist = std::uniform_real_distribution<float>(0, 1);
    };

    float getNext() {
      return dist(mt);
    };
};


inline Vec3 sampleCosineHemisphere(float u, float v, float& pdf) {
  float y = 1 - 2*u;
  float r = std::sqrt(std::max(0.0f, 1 - y*y));
  float phi = 2*M_PI*v;
  pdf = y / M_PI;
  return Vec3(r*std::cos(phi), y, r*std::sin(phi));
}
#endif
