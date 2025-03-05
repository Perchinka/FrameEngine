#include "Engine.hpp"
#include "Logger.hpp"
#include "MeshGenerator.hpp"
#include "objects/Object.hpp"
#include "objects/PointLight.hpp"
#include "rendering/Camera.hpp"
#include "rendering/Mesh.hpp"
#include <cmath>
#include <vector>

constexpr float G = 0.05f;

constexpr float massMonkey = 100.0f;
constexpr float massSphere = 10.0f;
constexpr float massSphere2 = 10.0f;

class Simulation : public Engine {
private:
  Mesh *monkeyMesh;
  Mesh *gravityPlane;
  Object *monkey;
  Object *plane;
  Object *sphere;
  Object *sphere2;

  PointLight *pointLight1;

  float timeElapsed = 0.0f;

  Vector3 velocitySphere;
  Vector3 velocitySphere2;

  Camera &camera = renderer.getCamera();

  Vector3 computeAcceleration(const Vector3 &r_i, const Vector3 &r_j,
                              float m_j) {
    Vector3 diff = r_j - r_i;
    float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
    return diff.normalized() * (G * m_j / (distSq + 0.0001f));
  }

public:
  void on_start() override {
    monkeyMesh = Mesh::loadFromOBJ("demo_assets/monkey.obj");
    if (!monkeyMesh) {
      LOG(ERROR, "Error while loading .obj file");
    }

    // Create a large, high-resolution plane (20x20 units, 100 segments)
    gravityPlane = MeshGenerator::createPlane(20.f, 20.f, 100);
    plane = new Object(registry, gravityPlane);
    plane->setWireframe(true);
    plane->transform->position = Vector3(0.f, 0.f, 0.f);

    sphere = new Object(registry, MeshGenerator::createSphere());
    sphere->setColor(Vector3(1, 0, 1));
    sphere->transform->position = Vector3(-4.f, 0.f, 0.f);
    sphere->transform->scale = sphere->transform->scale * 0.3f;

    sphere2 = new Object(registry, MeshGenerator::createSphere());
    sphere2->setColor(Vector3(0, 0, 1));
    sphere2->transform->position = Vector3(4.f, 0.f, 0.f);
    sphere2->transform->scale = sphere2->transform->scale * 0.3f;

    monkey = new Object(registry, monkeyMesh);
    monkey->setColor(Vector3(0, 1, 0.5f));
    monkey->transform->position = Vector3(0.f, 0.f, 0.f);
    monkey->transform->scale = monkey->transform->scale * 0.5f;
    monkey->rotate(Vector3(0, 1, 0), 180);

    renderer.submit(plane);
    renderer.submit(sphere);
    renderer.submit(sphere2);
    renderer.submit(monkey);

    pointLight1 = new PointLight(registry, Vector3(5.0f, 5.0f, 0.0f),
                                 Vector3(1, 1, 0), 0.7f);
    PointLight *pointLight2 = new PointLight(
        registry, Vector3(0.0f, 3.0f, 0.0f), Vector3(1, 1, 1), 1.f);
    renderer.submitLight(pointLight1);
    renderer.submitLight(pointLight2);

    camera.transform->position = Vector3(0.f, 5.f, -10.f);
    camera.setProjection(60.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    camera.lookAt(plane->transform->position);

    velocitySphere = Vector3(0.f, 0.f, 0.835f);
    velocitySphere2 = Vector3(0.f, 0.f, -0.835f);
  }

  void fixed_update(float dt) override {
    float deltaTime = dt;
    timeElapsed += deltaTime;

    {
      Vector3 acc = Vector3(0.f, 0.f, 0.f);
      acc = acc + computeAcceleration(sphere->transform->position,
                                      monkey->transform->position, massMonkey);
      acc =
          acc + computeAcceleration(sphere->transform->position,
                                    sphere2->transform->position, massSphere2);
      velocitySphere = velocitySphere + acc * deltaTime;
      sphere->transform->position =
          sphere->transform->position + velocitySphere * deltaTime;
    }

    {
      Vector3 acc = Vector3(0.f, 0.f, 0.f);
      acc = acc + computeAcceleration(sphere2->transform->position,
                                      monkey->transform->position, massMonkey);
      acc = acc + computeAcceleration(sphere2->transform->position,
                                      sphere->transform->position, massSphere);
      velocitySphere2 = velocitySphere2 + acc * deltaTime;
      sphere2->transform->position =
          sphere2->transform->position + velocitySphere2 * deltaTime;
    }

    camera.lookAt(monkey->transform->position);

    const float curvatureFactor = 0.2f;
    const float sigma = 2.0f;
    auto computeDeformation = [sigma, curvatureFactor](const Vector3 &pos,
                                                       float m, float vx,
                                                       float vz) -> float {
      float dx = vx - pos.x;
      float dz = vz - pos.z;
      float d2 = dx * dx + dz * dz;
      return -curvatureFactor * m * std::exp(-d2 / (sigma * sigma));
    };

    if (gravityPlane) {
      std::vector<float> &vertices = gravityPlane->getVertexData();
      size_t vertexCount = vertices.size() / 6;
      for (size_t i = 0; i < vertexCount; i++) {
        float x = vertices[i * 6 + 0];
        float z = vertices[i * 6 + 2];
        float deformation = 0.0f;
        deformation +=
            computeDeformation(monkey->transform->position, massMonkey, x, z);
        deformation +=
            computeDeformation(sphere->transform->position, massSphere, x, z);
        deformation +=
            computeDeformation(sphere2->transform->position, massSphere2, x, z);
        vertices[i * 6 + 1] = deformation;
      }
      gravityPlane->updateBuffer();
    }
  }
};

int main() {
  Simulation sim;
  sim.init();
  sim.run();
  return 0;
}
