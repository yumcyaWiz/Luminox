#ifndef INTEGRATOR_H
#define INTEGRATOR_H
#include <memory>
#include "image.h"
#include "camera.h"
#include "scene.h"


class Integrator {
  public:
    Image image;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Sampler> sampler;

    Integrator(const Image& _image, const std::shared_ptr<Camera>& _camera, const std::shared_ptr<Sampler>& _sampler) : image(_image), camera(_camera), sampler(_sampler) {};

    virtual Vec3 integrate(const Ray& ray, const Scene& scene) = 0;

    void render(const Scene& scene) {
      for(int i = 0; i < image.height; i++) {
        for(int j = 0; j < image.width; j++) {
          float rx = sampler->getNext();
          float ry = sampler->getNext();
          float u = (2*(j + rx) - image.width) / image.height;
          float v = (2*(i + ry) - image.height) / image.height;

          Ray ray = camera->getRay(u, v);
          Vec3 color = this->integrate(ray, scene);

          image.addPixel(i, j, color);
        }
      }
    };
};


class PurePathTracing : public Integrator {
  public:
    PurePathTracing(const Image& _image, const std::shared_ptr<Camera>& _camera, const std::shared_ptr<Sampler>& _sampler) : Integrator(_image, _camera, _sampler) {};

    Vec3 integrate(const Ray& ray, const Scene& scene) {
    };
};
#endif
