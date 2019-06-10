#ifndef VEC3_H
#define VEC3_H
#include <cmath>
#include <iostream>
class Vec3 {
  public:
    float x;
    float y;
    float z;

    Vec3() { x = y = z = 0; };
    Vec3(float _x) { x = y = z = _x; };
    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};

    Vec3 operator-() const {
      return Vec3(-x, -y, -z);
    };

    Vec3& operator+=(const Vec3& v) {
      x += v.x;
      y += v.y;
      z += v.z;
      return *this;
    };
    Vec3& operator-=(const Vec3& v) {
      x -= v.x;
      y -= v.y;
      z -= v.z;
      return *this;
    };
    Vec3& operator*=(const Vec3& v) {
      x *= v.x;
      y *= v.y;
      z *= v.z;
      return *this;
    };
    Vec3& operator/=(const Vec3& v) {
      x /= v.x;
      y /= v.y;
      z /= v.z;
      return *this;
    };

    float length() const {
      return std::sqrt(x*x + y*y + z*z);
    };
    float length2() const {
      return x*x + y*y + z*z;
    };
};
inline Vec3 operator+(const Vec3& v1, const Vec3& v2) {
  return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
inline Vec3 operator+(const Vec3& v1, float k) {
  return Vec3(v1.x + k, v1.y + k, v1.z + k);
}
inline Vec3 operator+(float k, const Vec3& v2) {
  return v2 + k;
}


inline Vec3 operator-(const Vec3& v1, const Vec3& v2) {
  return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
inline Vec3 operator-(const Vec3& v1, float k) {
  return Vec3(v1.x - k, v1.y - k, v1.z - k);
}
inline Vec3 operator-(float k, const Vec3& v2) {
  return Vec3(k - v2.x, k - v2.y, k - v2.z);
}


inline Vec3 operator*(const Vec3& v1, const Vec3& v2) {
  return Vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}
inline Vec3 operator*(const Vec3& v1, float k) {
  return Vec3(v1.x * k, v1.y * k, v1.z * k);
}
inline Vec3 operator*(float k, const Vec3& v2) {
  return v2 * k;
}


inline Vec3 operator/(const Vec3& v1, const Vec3& v2) {
  return Vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}
inline Vec3 operator/(const Vec3& v1, float k) {
  return Vec3(v1.x / k, v1.y / k, v1.z / k);
}
inline Vec3 operator/(float k, const Vec3& v2) {
  return Vec3(k / v2.x, k / v2.y, k / v2.z);
}


inline std::ostream& operator<<(std::ostream& stream, const Vec3& v) {
  stream << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return stream;
}


inline float dot(const Vec3& v1, const Vec3& v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
inline Vec3 cross(const Vec3& v1, const Vec3& v2) {
  return Vec3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}


inline Vec3 normalize(const Vec3& v) {
  return v/v.length();
}


inline void orthonormalBasis(const Vec3& v1, Vec3& v2, Vec3& v3) {
  v2 = Vec3(1, 0, 0);
  if(std::abs(v1.x) > 0.9) v2 = Vec3(0, 1, 0);

  v2 = normalize(v2 - dot(v1, v2)*v1);
  v3 = normalize(cross(v1, v2));
}


inline Vec3 world2local(const Vec3& p, const Vec3& v1, const Vec3& v2, const Vec3& v3) {
  return Vec3(dot(p, v1), dot(p, v2), dot(p, v3));
}
inline Vec3 local2world(const Vec3& p, const Vec3& v1, const Vec3& v2, const Vec3& v3) {
  Vec3 a = Vec3(v1.x, v2.x, v3.x);
  Vec3 b = Vec3(v1.y, v2.y, v3.y);
  Vec3 c = Vec3(v1.z, v2.z, v3.z);
  return Vec3(dot(p, a), dot(p, b), dot(p, c));
}
#endif
