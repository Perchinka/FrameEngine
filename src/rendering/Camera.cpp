#include "rendering/Camera.hpp"

/**
 * @brief Computes the view matrix based on position and front vector.
 * @return View matrix for transformations
 */
Matrix4 Camera::getViewMatrix() const {
  return Matrix4::lookAt(position, position + front, up);
}

/**
 * @brief Returns the perspective projection matrix.
 * @return Projection matrix
 */
Matrix4 Camera::getProjectionMatrix() const {
  return Matrix4::perspective(fov, aspectRatio, nearPlane, farPlane);
}

/**
 * @brief Sets the camera position.
 * @param newPosition The new camera position
 */
void Camera::setPosition(const Vector3 &newPosition) { position = newPosition; }

/**
 * @brief Updates the camera direction.
 * @param newDirection The new forward direction
 */
void Camera::setDirection(const Vector3 &newDirection) {
  front = newDirection.normalized();
  right = front.cross(worldUp).normalized();
  up = right.cross(front).normalized();
}

/**
 * @brief Rotates the camera to look at a target point.
 * @param target Target position
 */
void Camera::lookAt(const Vector3 &target) {
  Vector3 direction = (target - position).normalized();
  if (direction.magnitude() == 0)
    return; // Prevent divide by zero

  front = direction;
  right = front.cross(worldUp).normalized();
  up = right.cross(front).normalized();
}
