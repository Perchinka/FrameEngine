/**
 * @file Engine.hpp
 * @brief Defines the core Engine class responsible for running simulation.
 */

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Registry.hpp"
#include "Window.hpp"
#include "rendering/Renderer.hpp"

/**
 * @class Engine
 * @brief Core engine class that manages the main loop, physics, and rendering.
 *
 * The Engine serves as the entry point for simulations and games.
 * It uses a fixed timestep update system
 *
 * Users should extend this class and override:
 * - on_start(): For initialization logic.
 * - fixed_update(float dt): For physics and game logic updates.
 */
class Engine {
private:
  // TODO Make it variable
  const float fixedTimeStep = 1.0f / 60.0f;
  /// To handle time difference between frames
  float accumulator = 0.0f;
  bool isRunning = true;

protected:
  /// ECS registry that stores and manages all entities and components
  Registry registry;
  Window window;
  Renderer renderer;

public:
  Engine();
  virtual ~Engine();

  /**
   * @brief Bootstrap
   * This function should be called before 'run()' to ensure all components
   * are properly set up (e.g., renderer, ECS initialization, etc).
   */
  void init();

  /**
   * @brief Starts the main game loop.
   */
  void run();

  /**
   * @brief Stops the engine gracefully.
   */
  void stop();

  // ===== User Defined Funcitons =====

  /**
   * @brief User-defined logic that updates every fixed timestep.
   * @param dt The fixed time step duration.
   *
   * Users should override this function to define physics updates
   */
  virtual void fixed_update(float dt);

  /**
   * @brief Called once at the start of execution.
   *
   * Users should override this function to handle initialization, entity
   * creation etc
   */
  virtual void on_start();
};

#endif // ENGINE_HPP
