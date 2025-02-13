#include "objects/Object.hpp"
#include "rendering/Renderer.hpp"
#include <iostream>

void Object::render(Renderer &renderer) {
  Shader *shader = renderer.getShader();
  if (!shader)
    return;

  if (!mesh) {
    return;
  }

  Matrix4 modelMatrix = transform->get_transformation_matrix();

  shader->setUniformVec3("objectColor", color);
  shader->setUniformMat4("model", modelMatrix);
  mesh->draw();
}
