/**
 * @file PhysicsSystem.hpp
 * @brief Defines the PhysicsSystem
 *
 * The PhysicsSystem processes all entities with 'PhysicsComponent' (stores
 * velocity, acceleration, and mass).
 *
 * It applies acceleration to velocity, then integrates velocity into position.
 * This system should be called every frame with a fixed timestamp to maintain
 * stable physics updates.
 *
 * In the future will extend it with collision checks and other physics stuff
 */

#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "Registry.hpp"
#include "components/PhysicsComponent.hpp"
#include "components/TransformComponent.hpp"

class PhysicsSystem {
public:
  static void update(Registry &registry, float dt) {
    for (EntityID entity :
         registry.get_entities_with_component<PhysicsComponent>()) {
      if (!registry.has_component<TransformComponent>(entity))
        continue;

      auto &transform = registry.get_component<TransformComponent>(entity);
      auto &physics = registry.get_component<PhysicsComponent>(entity);

      physics.velocity = physics.velocity + physics.acceleration * dt;

      transform.position = transform.position + physics.velocity * dt;
    }
  }
};

#endif // PHYSICS_SYSTEM_HPP
