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
  auto camera = std::make_shared<PinholeCamera>(Vec3(0, 1, -3), Vec3(0, 0, 1));
  auto sampler = std::make_shared<UniformSampler>();

  auto plane = std::make_shared<Plane>(Vec3(0, 0, 0), 3, 3, Vec3(0, 1, 0), Vec3(1, 0, 0));
  auto sphere = std::make_shared<Sphere>(Vec3(0, 1, 0), 1);

  auto plane_prim = std::make_shared<Primitive>(plane, std::make_shared<Diffuse>(std::make_shared<Mat>(Vec3(0.9))), nullptr);
  auto sphere_prim = std::make_shared<Primitive>(sphere, std::make_shared<Diffuse>(std::make_shared<Mat>(Vec3(0.9))), nullptr);

  Scene scene;
  scene.add(plane_prim);
  scene.add(sphere_prim);

  PurePathTracing integrator(image, camera, sampler, 1);
}

