#ifndef INTEGRATOR_H
#define INTEGRATOR_H
#include <memory>
#include <chrono>
#include <omp.h>
#include "image.h"
#include "camera.h"
#include "scene.h"
#include "util.h"


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
          accumulated_color += throughput * Vec3(1, 1, 1);
          break;
        }
      }
      return accumulated_color;
    };


    void render(const Scene& scene) {
      auto start_time = std::chrono::system_clock::now();
      for(int k = 0; k < samples; k++) {
#pragma omp parallel for schedule(dynamic, 1)
        for(int i = 0; i < image->height; i++) {
          for(int j = 0; j < image->width; j++) {
            float u = (2*(j + sampler->getNext()) - image->width)/image->height;
            float v = (2*(i + sampler->getNext()) - image->height)/image->height;
            Ray ray = camera->getRay(u, v);
            image->addPixel(i, j, this->integrate(ray, scene));

            if(omp_get_thread_num() == 0) {
              int index = j + image->width*i + image->width*image->height*k;
              std::cout << progressbar(index, image->width*image->height*samples) << " " << percentage(index, image->width*image->height*samples) << "\r" << std::flush;
            }
          }
        }
      }
      auto end_time = std::chrono::system_clock::now();
      auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

      std::cout << "Rendering finished in " << msec << "ms" << std::endl;

      image->divide(samples);
      image->ppm_output("output.ppm");
    };
};


class NEEPathTracing : public Integrator {
  public:
    int samples;
    int maxDepth;

    NEEPathTracing(const std::shared_ptr<Image>& _image, const std::shared_ptr<Camera>& _camera, const std::shared_ptr<Sampler>& _sampler, int _samples, int _maxDepth=100) : Integrator(_image, _camera, _sampler), samples(_samples), maxDepth(_maxDepth) {};

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
          //Corner Case
          if(i == 0 && res.hitPrimitive->hasLight()) {
            accumulated_color += res.hitPrimitive->light->Le();
          }

          //Generate Local Coordinate Vectors
          Vec3 n = res.hitNormal;
          Vec3 s, t;
          orthonormalBasis(n, s, t);
          Vec3 wo = -ray.direction;
          Vec3 wo_local = world2local(wo, s, n, t);

          //light sampling
          for(auto l : scene.lights) {
            Vec3 sampledPos;
            float pdf_A;
            Vec3 le = l->sample(res, *sampler, sampledPos, pdf_A);

            Vec3 wi = normalize(sampledPos - res.hitPos);
            Hit shadow_res;
            Ray shadowRay(res.hitPos, wi);
            if(scene.intersect(shadowRay, shadow_res) && shadow_res.hitPrimitive->light == l) {
              float pdf_w = (sampledPos - res.hitPos).length2() / std::abs(dot(-shadowRay.direction, shadow_res.hitNormal)) * pdf_A;
              Vec3 wi_local = world2local(wi, s, n, t);
              Vec3 brdf = res.hitPrimitive->material->BRDF(res, wo_local, wi_local);
              accumulated_color += throughput * brdf * absCosTheta(wi_local) * le / pdf_w;
            }
          }

          //BRDF sampling for next ray
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
        else {
          accumulated_color += throughput * Vec3(0, 0, 0);
          break;
        }
      }
      return accumulated_color;
    };


    void render(const Scene& scene) {
      auto start_time = std::chrono::system_clock::now();
      for(int k = 0; k < samples; k++) {
#pragma omp parallel for schedule(dynamic, 1)
        for(int i = 0; i < image->height; i++) {
          for(int j = 0; j < image->width; j++) {
            float u = (2*(j + sampler->getNext()) - image->width)/image->height;
            float v = (2*(i + sampler->getNext()) - image->height)/image->height;
            Ray ray = camera->getRay(u, v);
            image->addPixel(i, j, this->integrate(ray, scene));

            if(omp_get_thread_num() == 0) {
              int index = j + image->width*i + image->width*image->height*k;
              std::cout << progressbar(index, image->width*image->height*samples) << " " << percentage(index, image->width*image->height*samples) << "\r" << std::flush;
            }
          }
        }
      }
      auto end_time = std::chrono::system_clock::now();
      auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

      std::cout << "Rendering finished in " << msec << "ms" << std::endl;

      image->divide(samples);
      image->ppm_output("output.ppm");
    };
};
#endif
