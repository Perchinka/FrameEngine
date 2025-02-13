#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "math/Matrix4.hpp"
#include "math/Vector3.hpp"

class Camera {
private:
  Vector3 right;
  Vector3 worldUp;

public:
  Vector3 position;
  Vector3 front;
  Vector3 up;

  float fov;
  float aspectRatio;
  float nearPlane;
  float farPlane;

  Camera(float aspectRatio)
      : position(0.0f, 0.0f, 3.0f), front(0.0f, 0.0f, -1.0f),
        up(0.0f, 1.0f, 0.0f), worldUp(0.0f, 1.0f, 0.0f), fov(45.0f),
        aspectRatio(aspectRatio), nearPlane(0.1f), farPlane(100.0f) {
    right = front.cross(worldUp).normalized();
  }

  Matrix4 getViewMatrix() const;
  Matrix4 getProjectionMatrix() const;

  void setPosition(const Vector3 &newPosition);
  void setDirection(const Vector3 &newDirection);
  void lookAt(const Vector3 &target);
};

#endif // CAMERA_HPP
