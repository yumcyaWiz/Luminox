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


int main() {
  Image img(512, 512);
  PinholeCamera cam(Vec3(0, 1, -3), Vec3(0, 0, 1));

  auto plane = std::make_shared<Plane>(Vec3(0, 0, 0), 3, 3, Vec3(0, 1, 0), Vec3(1, 0, 0));
  auto sphere = std::make_shared<Sphere>(Vec3(0, 1, 0), 1);

  auto plane_prim = std::make_shared<Primitive>(plane, std::make_shared<Diffuse>(std::make_shared<Mat>(Vec3(0.9))), nullptr);
  auto sphere_prim = std::make_shared<Primitive>(sphere, std::make_shared<Diffuse>(std::make_shared<Mat>(Vec3(0.9))), nullptr);

  Scene scene;
  scene.add(plane_prim);
  scene.add(sphere_prim);

  for(int i = 0; i < img.height; i++) {
    for(int j = 0; j < img.width; j++) {
      float u = (2.0*j - img.width)/img.height;
      float v = (2.0*i - img.height)/img.height;

      Ray ray = cam.getRay(u, v);
      Hit res;
      if(scene.intersect(ray, res)) {
        img.setPixel(i, j, Vec3(1));
      }
      else {
        img.setPixel(i, j, Vec3(0));
      }
    }
  }

  img.ppm_output("output.ppm");
}

