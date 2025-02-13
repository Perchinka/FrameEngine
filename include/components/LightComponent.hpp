#ifndef LIGHT_COMPONENT_HPP
#define LIGHT_COMPONENT_HPP

#include "math/Vector3.hpp"

enum class LightType { DIRECTIONAL, POINT, SPOTLIGHT };

struct LightComponent {
  LightType type;
  Vector3 color;
  float intensity;

  LightComponent(LightType t = LightType::DIRECTIONAL,
                 Vector3 col = Vector3(1.0f, 1.0f, 1.0f),
                 float intens = 1.0f)
      : type(t), color(col), intensity(intens) {}
};

#endif // LIGHT_COMPONENT_HPP
