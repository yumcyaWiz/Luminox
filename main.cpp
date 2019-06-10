#include <iostream>
#include <memory>
#include "vec3.h"
#include "image.h"
#include "ray.h"
#include "hit.h"
#include "shape.h"
#include "primitive.h"
#include "camera.h"
#include "scene.h"
#include "sampler.h"
#include "material.h"
#include "texture.h"
#include "light.h"
#include "integrator.h"


int main() {
  auto image = std::make_shared<Image>(512, 512);
  auto camera = std::make_shared<PinholeCamera>(Vec3(1, 1, -4), Vec3(0, 0, 1));
  auto sampler = std::make_shared<UniformSampler>();

  auto plane = std::make_shared<Plane>(Vec3(0, 0, 0), 100, 100, Vec3(0, 1, 0), Vec3(1, 0, 0));
  auto sphere1 = std::make_shared<Sphere>(Vec3(0, 1, 0), 1);
  auto sphere2 = std::make_shared<Sphere>(Vec3(2, 1, 2), 1);
  auto sphere3 = std::make_shared<Sphere>(Vec3(4, 1, 4), 1);

  auto plane_prim = std::make_shared<Primitive>(plane, std::make_shared<Diffuse>(std::make_shared<Mat>(Vec3(0.8))), nullptr);
  auto sphere_prim1 = std::make_shared<Primitive>(sphere1, std::make_shared<Diffuse>(std::make_shared<Mat>(Vec3(0.8, 0.2, 0.2))), nullptr);
  auto sphere_prim2 = std::make_shared<Primitive>(sphere2, std::make_shared<Diffuse>(std::make_shared<Mat>(Vec3(0.2, 0.8, 0.2))), nullptr);
  auto sphere_prim3 = std::make_shared<Primitive>(sphere3, std::make_shared<Diffuse>(std::make_shared<Mat>(Vec3(0.2, 0.2, 0.8))), nullptr);

  Scene scene;
  scene.add(plane_prim);
  scene.add(sphere_prim1);
  scene.add(sphere_prim2);
  scene.add(sphere_prim3);

  PurePathTracing integrator(image, camera, sampler, 100);
  integrator.render(scene);
}

