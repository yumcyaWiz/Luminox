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
  auto camera = std::make_shared<PinholeCamera>(Vec3(0, 3, -8), Vec3(0, 0, 1));
  auto sampler = std::make_shared<UniformSampler>();

  auto floor = std::make_shared<Plane>(Vec3(0, 0, 0), 6, 6, Vec3(0, 1, 0), Vec3(1, 0, 0));
  auto right_wall = std::make_shared<Plane>(Vec3(3, 3, 0), 6, 6, Vec3(-1, 0, 0), Vec3(0, 1, 0));
  auto left_wall = std::make_shared<Plane>(Vec3(-3, 3, 0), 6, 6, Vec3(1, 0, 0), Vec3(0, 1, 0));
  auto ceil = std::make_shared<Plane>(Vec3(0, 6, 0), 6, 6, Vec3(0, -1, 0), Vec3(1, 0, 0));
  auto forward_wall = std::make_shared<Plane>(Vec3(0, 3, 3), 6, 6, Vec3(0, 0, -1), Vec3(1, 0, 0));
  auto sphere1 = std::make_shared<Sphere>(Vec3(-1.5, 1.5, 1.5), 1.5);
  auto sphere2 = std::make_shared<Sphere>(Vec3(1.5, 1.5, -1.0), 1.5);

  auto white1 = std::make_shared<Mat>(Vec3(0.8));
  auto white2 = std::make_shared<Mat>(Vec3(0.99));
  auto red = std::make_shared<Mat>(Vec3(0.8, 0.2, 0.2));
  auto green = std::make_shared<Mat>(Vec3(0.2, 0.8, 0.2));

  auto floor_prim = std::make_shared<Primitive>(floor, std::make_shared<Diffuse>(white1), nullptr);
  auto right_wall_prim = std::make_shared<Primitive>(right_wall, std::make_shared<Diffuse>(red), nullptr);
  auto left_wall_prim = std::make_shared<Primitive>(left_wall, std::make_shared<Diffuse>(green), nullptr);
  auto ceil_prim = std::make_shared<Primitive>(ceil, std::make_shared<Diffuse>(white1), std::make_shared<AreaLight>(Vec3(1.5), ceil));
  auto forward_wall_prim = std::make_shared<Primitive>(forward_wall, std::make_shared<Diffuse>(white1), nullptr);
  auto sphere1_prim = std::make_shared<Primitive>(sphere1, std::make_shared<Diffuse>(white1), nullptr);
  auto sphere2_prim = std::make_shared<Primitive>(sphere2, std::make_shared<Diffuse>(white1), nullptr);

  Scene scene;
  scene.add(floor_prim);
  scene.add(right_wall_prim);
  scene.add(left_wall_prim);
  scene.add(ceil_prim);
  scene.add(forward_wall_prim);
  scene.add(sphere1_prim);
  scene.add(sphere2_prim);

  NEEPathTracing integrator(image, camera, sampler, 10);
  integrator.render(scene);
}
