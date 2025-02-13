#ifndef MATRIX4_HPP
#define MATRIX4_HPP

#include "Quaternion.hpp"
#include "Vector3.hpp"

struct Matrix4 {
  float m[4][4];

  Matrix4() {
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        m[i][j] = (i == j) ? 1.0f : 0.0f; // Identity matrix
  }

  static Matrix4 from_rotation(const Vector3 &axis, float angleDegrees) {
    float angle = angleDegrees * M_PI / 180.0f; // Convert to radians
    Quaternion q = Quaternion::from_axis_angle(axis, angle);
    return q.toMatrix();
  }

  // Apply matrix transformation to a vector
  Vector3 transform(const Vector3 &v) const {
    return {v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0],
            v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1],
            v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2]};
  }

  Matrix4 operator*(const Matrix4 &other) const {
    Matrix4 result;

    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        result.m[row][col] = 0.0f;
        for (int i = 0; i < 4; i++) {
          result.m[row][col] += m[row][i] * other.m[i][col];
        }
      }
    }

    return result;
  }

  // Create a translation matrix from a Vector3
  static Matrix4 from_translation(const Vector3 &pos) {
    Matrix4 mat;

    mat.m[3][0] = pos.x;
    mat.m[3][1] = pos.y;
    mat.m[3][2] = pos.z;

    return mat;
  }

  // Create a scaling matrix from a Vector3
  static Matrix4 from_scaling(const Vector3 &s) {
    Matrix4 mat;

    mat.m[0][0] = s.x;
    mat.m[1][1] = s.y;
    mat.m[2][2] = s.z;

    return mat;
  }

  // Create a view matrix (Camera LookAt)
  static Matrix4 lookAt(const Vector3 &eye, const Vector3 &target,
                        const Vector3 &up) {
    Vector3 f = (target - eye).normalized(); // Forward direction
    Vector3 r = up.cross(f).normalized();    // Right direction
    Vector3 u = f.cross(r).normalized();     // Corrected Up direction

    Matrix4 mat;

    mat.m[0][0] = r.x;
    mat.m[1][0] = r.y;
    mat.m[2][0] = r.z;
    mat.m[3][0] = -r.dot(eye);
    mat.m[0][1] = u.x;
    mat.m[1][1] = u.y;
    mat.m[2][1] = u.z;
    mat.m[3][1] = -u.dot(eye);
    mat.m[0][2] = -f.x;
    mat.m[1][2] = -f.y;
    mat.m[2][2] = -f.z;
    mat.m[3][2] = f.dot(eye);
    mat.m[0][3] = 0;
    mat.m[1][3] = 0;
    mat.m[2][3] = 0;
    mat.m[3][3] = 1;

    return mat;
  }

  // Create a perspective projection matrix
  static Matrix4 perspective(float fov, float aspect, float near, float far) {
    float tanHalfFOV = tan(fov * 0.5f * M_PI / 180.0f);
    Matrix4 mat;
    mat.m[0][0] = 1.0f / (aspect * tanHalfFOV);
    mat.m[1][1] = 1.0f / tanHalfFOV;
    mat.m[2][2] = -(far + near) / (far - near);
    mat.m[2][3] = -1.0f;
    mat.m[3][2] = -(2.0f * far * near) / (far - near);
    mat.m[3][3] = 0.0f;
    return mat;
  }
};

#endif // MATRIX4_HPP
