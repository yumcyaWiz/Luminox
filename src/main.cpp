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
  auto camera = std::make_shared<PinholeCamera>(Vec3(278, 273, -900), Vec3(0, 0, 1), 45);
  auto sampler = std::make_shared<UniformSampler>();

  auto floor = std::make_shared<Plane>(Vec3(0, 0, 0), Vec3(0, 0, 559.2), Vec3(556, 0, 0));
  auto right_wall = std::make_shared<Plane>(Vec3(0, 0, 0), Vec3(0, 548.8, 0), Vec3(0, 0, 559.2));
  auto left_wall = std::make_shared<Plane>(Vec3(556, 0, 0), Vec3(0, 0, 559.2), Vec3(0, 548.8, 0));
  auto ceil = std::make_shared<Plane>(Vec3(0, 548.8, 0), Vec3(556, 0, 0), Vec3(0, 0, 559.2));
  auto forward_wall = std::make_shared<Plane>(Vec3(0, 0, 559.2), Vec3(0, 548.8, 0), Vec3(556, 0, 0));
  auto shortblock1 = std::make_shared<Plane>(Vec3(130, 165, 65), Vec3(-48, 0, 160), Vec3(160, 0, 49));
  auto shortblock2 = std::make_shared<Plane>(Vec3(290, 0, 114), Vec3(0, 165, 0), Vec3(-50, 0, 158));
  auto shortblock3 = std::make_shared<Plane>(Vec3(130, 0, 65), Vec3(0, 165, 0), Vec3(160, 0, 49));
  auto shortblock4 = std::make_shared<Plane>(Vec3(82, 0, 225), Vec3(0, 165, 0), Vec3(48, 0, -160));
  auto shortblock5 = std::make_shared<Plane>(Vec3(240, 0, 272), Vec3(0, 165, 0), Vec3(-158, 0, -47));
  auto tallblock1 = std::make_shared<Plane>(Vec3(423, 330, 247), Vec3(-158, 0, 49), Vec3(49, 0, 159));
  auto tallblock2 = std::make_shared<Plane>(Vec3(423, 0, 247), Vec3(0, 330, 0), Vec3(49, 0, 159));
  auto tallblock3 = std::make_shared<Plane>(Vec3(472, 0, 406), Vec3(0, 330, 0), Vec3(-158, 0, 50));
  auto tallblock4 = std::make_shared<Plane>(Vec3(314, 0, 456), Vec3(0, 330, 0), Vec3(-49, 0, -160));
  auto tallblock5 = std::make_shared<Plane>(Vec3(265, 0, 296), Vec3(0, 330, 0), Vec3(158, 0, -49));
  auto light = std::make_shared<Plane>(Vec3(343, 548.6, 227), Vec3(-130, 0, 0), Vec3(0, 0, 105));

  auto check = std::make_shared<Checkerboard>(Vec3(0.8), Vec3(0.1), 100);
  auto white1 = std::make_shared<Mat>(Vec3(0.8));
  auto white2 = std::make_shared<Mat>(Vec3(0.99));
  auto red = std::make_shared<Mat>(Vec3(0.8, 0.05, 0.05));
  auto green = std::make_shared<Mat>(Vec3(0.05, 0.8, 0.05));

  auto floor_prim = std::make_shared<Primitive>(floor, std::make_shared<Diffuse>(white1), nullptr);
  auto right_wall_prim = std::make_shared<Primitive>(right_wall, std::make_shared<Diffuse>(green), nullptr);
  auto left_wall_prim = std::make_shared<Primitive>(left_wall, std::make_shared<Diffuse>(red), nullptr);
  auto ceil_prim = std::make_shared<Primitive>(ceil, std::make_shared<Diffuse>(white1), nullptr);
  auto forward_wall_prim = std::make_shared<Primitive>(forward_wall, std::make_shared<Diffuse>(white1), nullptr);
  auto shortblock1_prim = std::make_shared<Primitive>(shortblock1, std::make_shared<Diffuse>(white1), nullptr);
  auto shortblock2_prim = std::make_shared<Primitive>(shortblock2, std::make_shared<Diffuse>(white1), nullptr);
  auto shortblock3_prim = std::make_shared<Primitive>(shortblock3, std::make_shared<Diffuse>(white1), nullptr);
  auto shortblock4_prim = std::make_shared<Primitive>(shortblock4, std::make_shared<Diffuse>(white1), nullptr);
  auto shortblock5_prim = std::make_shared<Primitive>(shortblock5, std::make_shared<Diffuse>(white1), nullptr);
  auto tallblock1_prim = std::make_shared<Primitive>(tallblock1, std::make_shared<Diffuse>(white1), nullptr);
  auto tallblock2_prim = std::make_shared<Primitive>(tallblock2, std::make_shared<Diffuse>(white1), nullptr);
  auto tallblock3_prim = std::make_shared<Primitive>(tallblock3, std::make_shared<Diffuse>(white1), nullptr);
  auto tallblock4_prim = std::make_shared<Primitive>(tallblock4, std::make_shared<Diffuse>(white1), nullptr);
  auto tallblock5_prim = std::make_shared<Primitive>(tallblock5, std::make_shared<Diffuse>(white1), nullptr);
  auto light_prim = std::make_shared<Primitive>(light, std::make_shared<Diffuse>(white1), std::make_shared<AreaLight>(0.1*Vec3(340, 190, 100), light));

  Scene scene;
  scene.add(floor_prim);
  scene.add(right_wall_prim);
  scene.add(left_wall_prim);
  scene.add(ceil_prim);
  scene.add(forward_wall_prim);
  scene.add(shortblock1_prim);
  scene.add(shortblock2_prim);
  scene.add(shortblock3_prim);
  scene.add(shortblock4_prim);
  scene.add(shortblock5_prim);
  scene.add(tallblock1_prim);
  scene.add(tallblock2_prim);
  scene.add(tallblock3_prim);
  scene.add(tallblock4_prim);
  scene.add(tallblock5_prim);
  scene.add(light_prim);

  NEEPathTracing integrator(image, camera, sampler, 1000);
  integrator.render(scene);
}
