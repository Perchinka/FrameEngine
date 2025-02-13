#include "rendering/Shader.hpp"
#include "Logger.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

std::string Shader::loadShaderSource(const std::string &filePath) {
  std::ifstream file(filePath);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

GLuint Shader::compileShader(const std::string &source, GLenum type) {
  GLuint shader = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char log[512];
    glGetShaderInfoLog(shader, 512, nullptr, log);
    LOG(ERROR, "Shader Compilation Error: %s", log);
  }

  return shader;
}

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
  std::string vertexSrc = loadShaderSource(vertexPath);
  std::string fragmentSrc = loadShaderSource(fragmentPath);

  GLuint vertexShader = compileShader(vertexSrc, GL_VERTEX_SHADER);
  GLuint fragmentShader = compileShader(fragmentSrc, GL_FRAGMENT_SHADER);

  programID = glCreateProgram();
  glAttachShader(programID, vertexShader);
  glAttachShader(programID, fragmentShader);
  glLinkProgram(programID);

  int success;
  glGetProgramiv(programID, GL_LINK_STATUS, &success);
  if (!success) {
    char log[512];
    glGetProgramInfoLog(programID, 512, nullptr, log);
    LOG(ERROR, "Shader Linking Error: %s", log);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

Shader::~Shader() { glDeleteProgram(programID); }

void Shader::bind() const { glUseProgram(programID); }

void Shader::unbind() const { glUseProgram(0); }

// ===== SetUniform =====

void Shader::setUniformFloat(const std::string &name, float value) const {
  GLint location = glGetUniformLocation(programID, name.c_str());
  if (location == -1) {
    LOG(WARNING, "Uniform '%s' not found!", name);
    return;
  }
  glUniform1f(location, value);
}

void Shader::setUniformVec3(const std::string &name,
                            const Vector3 &value) const {
  GLint location = glGetUniformLocation(programID, name.c_str());
  if (location == -1) {
    LOG(WARNING, "Uniform '%s' not found!", name);
    return;
  }
  glUniform3f(location, value.x, value.y, value.z);
}

void Shader::setUniformMat4(const std::string &name, const Matrix4 &mat) const {
  GLint location = glGetUniformLocation(programID, name.c_str());
  if (location == -1) {
    LOG(WARNING, "Uniform '%s' not found!", name);
  }
  glUniformMatrix4fv(location, 1, GL_FALSE, &mat.m[0][0]);
}
