#include "vec3.h"
#include "image.h"
#include "ray.h"
#include "hit.h"
#include "primitive.h"
#include "camera.h"


int main() {
  Image img(512, 512);
  PinholeCamera cam(Vec3(0, 0, -3), Vec3(0, 0, 1));
  Sphere sphere(Vec3(0, 0, 0), 1);

  for(int i = 0; i < img.height; i++) {
    for(int j = 0; j < img.width; j++) {
      float u = (2.0*i - img.height)/img.height;
      float v = (2.0*j - img.width)/img.height;

      Ray ray = cam.getRay(u, v);
      Hit res;
      if(sphere.intersect(ray, res)) {
        img.setPixel(i, j, Vec3(1));
      }
      else {
        img.setPixel(i, j, Vec3(0));
      }
    }
  }

  img.ppm_output("output.ppm");
}

