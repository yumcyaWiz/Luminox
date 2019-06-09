#include "vec3.h"
#include "image.h"
#include "ray.h"
#include "hit.h"
#include "primitive.h"


int main() {
  Image img(512, 512);
  for(int i = 0; i < img.height; i++) {
    for(int j = 0; j < img.width; j++) {
      img.setPixel(i, j, Vec3(float(i)/img.height, float(j)/img.width, 1.0));
    }
  }
  img.ppm_output("output.ppm");
}
