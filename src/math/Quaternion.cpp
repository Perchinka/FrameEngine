#include "math/Quaternion.hpp"
#include "math/Matrix4.hpp"

Matrix4 Quaternion::toMatrix() const {
  Matrix4 mat;
  float xx = x * x, xy = x * y, xz = x * z, xw = x * w;
  float yy = y * y, yz = y * z, yw = y * w;
  float zz = z * z, zw = z * w;

  mat.m[0][0] = 1 - 2 * (yy + zz);
  mat.m[0][1] = 2 * (xy - zw);
  mat.m[0][2] = 2 * (xz + yw);

  mat.m[1][0] = 2 * (xy + zw);
  mat.m[1][1] = 1 - 2 * (xx + zz);
  mat.m[1][2] = 2 * (yz - xw);

  mat.m[2][0] = 2 * (xz - yw);
  mat.m[2][1] = 2 * (yz + xw);
  mat.m[2][2] = 1 - 2 * (xx + yy);

  return mat;
};
