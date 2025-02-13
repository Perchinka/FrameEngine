/**
 * @file Shader.hpp
 * @brief Manages OpenGL shaders.
 */

#ifndef SHADER_HPP
#define SHADER_HPP

#include "math/Matrix4.hpp"
#include "math/Vector3.hpp"
#include <glad/glad.h>
#include <string>

class Shader {
private:
  GLuint programID;
  std::string loadShaderSource(const std::string &filePath);
  GLuint compileShader(const std::string &source, GLenum type);

public:
  Shader(const std::string &vertexPath, const std::string &fragmentPath);
  ~Shader();

  void bind() const;
  void unbind() const;

  void setUniformFloat(const std::string &name, float value) const;
  void setUniformVec3(const std::string &name, const Vector3 &value) const;
  void setUniformMat4(const std::string &name, const Matrix4 &mat) const;

  GLuint getProgramID() const { return programID; }
};

#endif // SHADER_HPP
