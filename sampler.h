#ifndef SAMPLER_H
#define SAMPLER_H
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
#endif
