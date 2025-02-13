#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "Object.hpp"
#include "math/Vector3.hpp"

/**
 * @class PointLight
 * @brief Represents a point light source as an Object in the ECS.
 */
class PointLight : public Object {
private:
  Vector3 color;
  float intensity;

public:
  PointLight(Registry &registry, const Vector3 &pos,
             const Vector3 &col = Vector3(1, 1, 1), float inten = 1.0f)
      : Object(registry, nullptr), color(col), intensity(inten) {
    transform->position = pos;
  }

  Vector3 getColor() const { return color; }
  void setColor(const Vector3 &col) { color = col; }

  float getIntensity() const { return intensity; }
  void setIntensity(float inten) { intensity = inten; }
};

#endif // POINT_LIGHT_HPP
