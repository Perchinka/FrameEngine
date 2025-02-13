/**
 * @file Renderer.hpp
 * @brief Manages the rendering pipeline and object submission.
 */

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "objects/Object.hpp"
#include "objects/PointLight.hpp"
#include "rendering/Camera.hpp"
#include "rendering/Mesh.hpp"
#include "rendering/Shader.hpp"
#include <memory>
#include <vector>

class Renderer {
private:
  static Shader *shader;
  static std::vector<Object *> renderQueue;
  static std::vector<PointLight *> lights;
  Camera camera;

public:
  Renderer();

  /**
   * @brief Initializes the rendering system.
   *
   * This method must be called before any other rendering function.
   */
  static void initialize();

  /**
   * @brief Clears the screen before rendering a new frame.
   *
   * This should be called at the beginning of each frame to clear
   * the previous frame's contents.
   */
  static void clear();

  /**
   * @brief Submits an object to the rendering queue.
   * @param mesh The mesh to render.
   * @param shader The shader to use.
   */
  static void submit(Object *obj);
  void clearObjects();

  static void submitLight(PointLight *light);

  /**
   * @brief Swaps the front and back buffers.
   *
   * This function should be called at the end of the frame to present
   * the rendered image on the screen.
   */
  void render();

  /**
   * @brief Shuts down the rendering system and releases resources.
   *
   * This method should be called when the application is closing
   * to properly clean up the rendering context.
   */
  static void shutdown();

  void setShader(Shader *shader);
  static Shader *getShader() { return shader; }
  Camera &getCamera();
};

#endif // RENDERER_HPP
