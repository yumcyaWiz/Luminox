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
  float theta = 0.5 * std::acos(1 - 2*u);
  float phi = 2*M_PI*v;
  float y = std::cos(theta);
  pdf = y / M_PI;
  return Vec3(std::cos(phi)*std::sin(theta), y, std::sin(phi)*std::sin(theta));
}
#endif
