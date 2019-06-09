#include <memory>
#include "vec3.h"
#include "image.h"
#include "ray.h"
#include "hit.h"
#include "primitive.h"
#include "camera.h"
#include "accel.h"


int main() {
  Image img(512, 512);
  PinholeCamera cam(Vec3(0, 1, -3), Vec3(0, 0, 1));

  Accel accel;
  accel.add(std::make_shared<Plane>(Vec3(0, 0, 0), 3, 3, Vec3(0, 1, 0), Vec3(1, 0, 0)));
  accel.add(std::make_shared<Sphere>(Vec3(0, 1, 0), 1));

  for(int i = 0; i < img.height; i++) {
    for(int j = 0; j < img.width; j++) {
      float u = (2.0*j - img.width)/img.height;
      float v = (2.0*i - img.height)/img.height;

      Ray ray = cam.getRay(u, v);
      Hit res;
      if(accel.intersect(ray, res)) {
        img.setPixel(i, j, Vec3(1));
      }
      else {
        img.setPixel(i, j, Vec3(0));
      }
    }
  }

  img.ppm_output("output.ppm");
}

