#ifndef INTEGRATOR_H
#define INTEGRATOR_H
#include <memory>
#include <omp.h>
#include "image.h"
#include "camera.h"
#include "scene.h"


class Integrator {
  public:
    std::shared_ptr<Image> image;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Sampler> sampler;

    Integrator(const std::shared_ptr<Image>& _image, const std::shared_ptr<Camera>& _camera, const std::shared_ptr<Sampler>& _sampler) : image(_image), camera(_camera), sampler(_sampler) {};

    virtual Vec3 integrate(const Ray& ray, const Scene& scene) = 0;
};


class PurePathTracing : public Integrator {
  public:
    int samples;
    int maxDepth;


    PurePathTracing(const std::shared_ptr<Image>& _image, const std::shared_ptr<Camera>& _camera, const std::shared_ptr<Sampler>& _sampler, int _samples, int _maxDepth=100) : Integrator(_image, _camera, _sampler), samples(_samples), maxDepth(_maxDepth) {};


    Vec3 integrate(const Ray& initRay, const Scene& scene) {
      Ray ray = initRay;
      Vec3 throughput(1, 1, 1);
      float roulette = 1;
      Vec3 accumulated_color(0, 0, 0);

      for(int i = 0; i < maxDepth; i++) {
        //Russian Roulette
        if(sampler->getNext() > roulette) {
          break;
        }
        else {
          throughput /= roulette;
          roulette *= 0.99;
        }

        Hit res;
        if(scene.intersect(ray, res)) {
          //When ray hits arealight
          if(res.hitPrimitive->hasLight()) {
            accumulated_color += throughput * res.hitPrimitive->light->Le();
          }

          //Generate Local Coordinate Vectors
          Vec3 n = res.hitNormal;
          Vec3 s, t;
          orthonormalBasis(n, s, t);
          Vec3 wo = -ray.direction;
          Vec3 wo_local = world2local(wo, s, n, t);

          //BRDF Sampling
          auto mat = res.hitPrimitive->material;
          Vec3 wi_local;
          float pdf_w;
          Vec3 brdf_value = mat->sample(wo_local, res, *sampler, wi_local, pdf_w);

          //Update throughput
          throughput *= brdf_value * absCosTheta(wi_local) / pdf_w;

          //Generate Next Ray
          Vec3 wi = local2world(wi_local, s, n, t);
          ray = Ray(res.hitPos, wi);
        }
        //When ray hits sky
        else {
          accumulated_color += throughput * Vec3(0, 0, 0);
          break;
        }
      }
      return accumulated_color;
    };


    void render(const Scene& scene) {
      for(int k = 0; k < samples; k++) {
#pragma omp parallel for schedule(dynamic, 1)
        for(int i = 0; i < image->height; i++) {
          for(int j = 0; j < image->width; j++) {
            float u = (2*(j + sampler->getNext()) - image->width)/image->height;
            float v = (2*(i + sampler->getNext()) - image->height)/image->height;
            Ray ray = camera->getRay(u, v);
            image->addPixel(i, j, this->integrate(ray, scene));
          }
        }
      }
      image->divide(samples);
      image->ppm_output("output.ppm");
    };
};
#endif
