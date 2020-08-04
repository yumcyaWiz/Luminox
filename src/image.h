#ifndef IMAGE_H
#define IMAGE_H
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "vec3.h"


float clamp(float x, float xmin, float xmax) {
  if(x < xmin) return xmin;
  else if(x > xmax) return xmax;
  else return x;
}


class Image {
  public:
    int width;
    int height;
    Vec3* pixels;

    Image(int _width, int _height) : width(_width), height(_height) {
      pixels = new Vec3[width*height];
    };
    ~Image() {
      delete[] pixels;
    };

    Vec3 getPixel(int i, int j) const {
      return pixels[j + i*width];
    };
    void setPixel(int i, int j, const Vec3& c) {
      pixels[j + i*width] = c;
    };
    void addPixel(int i, int j, const Vec3& c) {
      pixels[j + i*width] += c;
    };

    void divide(float k) {
      for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
          pixels[j + i*width] /= k;
        }
      }
    };

    void ppm_output(const std::string& filename) const {
      std::ofstream file(filename);
      file << "P3" << std::endl;
      file << width << " " << height << std::endl;
      file << "255" << std::endl;

      for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
          Vec3 c = this->getPixel(i, j);
          int r = clamp(255*std::pow(c.x, 1/2.2), 0, 255);
          int g = clamp(255*std::pow(c.y, 1/2.2), 0, 255);
          int b = clamp(255*std::pow(c.z, 1/2.2), 0, 255);
          file << r << " " << g << " " << b << std::endl;
        }
      }
    };
};
#endif
