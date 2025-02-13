#include "Logger.hpp"
#include "rendering/Mesh.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Mesh *Mesh::loadFromOBJ(const std::string &filePath) {
  std::vector<Vector3> tempVertices;
  std::vector<Vector3> tempNormals;
  std::vector<unsigned int> indices;

  std::vector<float> vertices;
  std::vector<float> normals;

  std::ifstream file(filePath);
  if (!file.is_open()) {
    LOG(ERROR, "Failed to open OBJ file: %s", filePath);
    return nullptr;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;

    if (prefix == "v") {
      float x, y, z;
      iss >> x >> y >> z;
      tempVertices.push_back(Vector3(x, y, z));
    } else if (prefix == "vn") {
      float x, y, z;
      iss >> x >> y >> z;
      tempNormals.push_back(Vector3(x, y, z));
    } else if (prefix == "f") {
      unsigned int vIdx[3], nIdx[3];
      char slash;

      for (int i = 0; i < 3; i++) {
        iss >> vIdx[i] >> slash >> slash >> nIdx[i];
        vIdx[i]--;
        nIdx[i]--;
        indices.push_back(vertices.size() / 3);

        vertices.push_back(tempVertices[vIdx[i]].x);
        vertices.push_back(tempVertices[vIdx[i]].y);
        vertices.push_back(tempVertices[vIdx[i]].z);

        normals.push_back(tempNormals[nIdx[i]].x);
        normals.push_back(tempNormals[nIdx[i]].y);
        normals.push_back(tempNormals[nIdx[i]].z);
      }
    }
  }

  return new Mesh(vertices, normals, indices);
}
