#include "Engine.hpp"
#include "Logger.hpp"
#include "objects/Object.hpp"
#include "rendering/Camera.hpp"
#include "rendering/Mesh.hpp"

Logger &logger = Logger::getInstance();

class Simulation : public Engine {
private:
  Mesh *cubeMesh;
  Object *cube1;
  Object *cube2;

  PointLight *pointLight;

  float timeElapsed = 0.0f;
  Camera &camera = renderer.getCamera();

public:
  void on_start() override {
    cubeMesh = Mesh::loadFromOBJ("demo_assets/monkey.obj");
    if (!cubeMesh) {
      LOG(ERROR, "Error while loading .obj file");
    }

    // And reuse it :D
    cube1 = new Object(registry, cubeMesh);
    cube1->transform->position = Vector3(0.0f, 0.0f, 0.0f);
    cube1->transform->scale = cube1->transform->scale * .3;
    cube1->rotate(Vector3(0, 1, 0), 180);

    cube2 = new Object(registry, cubeMesh);
    cube2->transform->scale = cube1->transform->scale * .5;
    cube2->transform->position = Vector3(1.5f, 2.0f, 3);
    cube2->setColor(Vector3(1, 0, 0));

    renderer.submit(cube1);
    renderer.submit(cube2);

    pointLight =
        new PointLight(registry, Vector3(5.0f, 5.0f, 0.0f),
                       Vector3(0xf9 / 256.f, 0xd7 / 256.f, 0x1c / 256.f), .7f);
    renderer.submitLight(pointLight);

    camera.setPosition(Vector3(0, 0, -2.0f));
    camera.lookAt(Vector3(0.0f, 0.0f, 0.0f));
  }

  void fixed_update(float dt) override {
    timeElapsed += dt;

    // cube1->rotate(Vector3(0.5f, 1.0f, 0.0f), dt * 50.0f);
    cube2->rotate(Vector3(1.0f, 1.0f, 2.0f), dt * 50.0f);
    cube2->move(Vector3(sin(timeElapsed) / 20.0f, 0, 0));

    float radius = .5;
    float ang_speed = 1;
    pointLight->transform->position =
        (Vector3(radius * cos(ang_speed * timeElapsed), 0,
                 radius * sin(ang_speed * timeElapsed)));
  }
};

int main() {
  Simulation sim;
  sim.init();
  sim.run();
  return 0;
}
