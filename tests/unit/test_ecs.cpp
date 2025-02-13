#define CATCH_CONFIG_MAIN

#include "Registry.hpp"
#include "catch2/catch_all.hpp"
#include "components/PhysicsComponent.hpp"
#include "components/TransformComponent.hpp"
#include "physics/PhysicsSystem.hpp"

TEST_CASE("EntityID creation and deletion", "[ECS]") {
  Registry registry;

  EntityID e1 = registry.create_entity();
  EntityID e2 = registry.create_entity();

  REQUIRE(e1 != e2); // Entities should have unique IDs

  registry.destroy_entity(e1);

  // Ensure that e1 no longer has any components (thus, it is "destroyed")
  REQUIRE_FALSE(registry.has_component<TransformComponent>(e1));
}

TEST_CASE("Component addition and retrieval", "[ECS]") {
  Registry registry;
  EntityID entity = registry.create_entity();

  // Add a TransformComponent to the entity
  TransformComponent transform = {{1.0f, 2.0f, 3.0f}};
  registry.add_component<TransformComponent>(entity, transform);

  // Verify that the component exists
  REQUIRE(registry.has_component<TransformComponent>(entity));

  // Check that the component data is correct
  REQUIRE(registry.get_component<TransformComponent>(entity).position.x ==
          Catch::Approx(1.0f));
  REQUIRE(registry.get_component<TransformComponent>(entity).position.y ==
          Catch::Approx(2.0f));
  REQUIRE(registry.get_component<TransformComponent>(entity).position.z ==
          Catch::Approx(3.0f));
}

TEST_CASE("Component removal", "[ECS]") {
  Registry registry;
  EntityID entity = registry.create_entity();

  registry.add_component<TransformComponent>(entity, {{1.0f, 2.0f, 3.0f}});
  registry.remove_component<TransformComponent>(entity);

  // The entity should no longer have this component
  REQUIRE_FALSE(registry.has_component<TransformComponent>(entity));
}

TEST_CASE("Query entities with a specific component", "[ECS]") {
  Registry registry;

  // Create two entities, but only add a component to one, test should return
  // only entities with components
  EntityID e1 = registry.create_entity();
  EntityID e2 = registry.create_entity();
  registry.add_component<TransformComponent>(e1, {{1.0f, 2.0f, 3.0f}});

  auto entities = registry.get_entities_with_component<TransformComponent>();

  REQUIRE(entities.size() == 1);
  REQUIRE(entities[0] == e1);
}

/*Not completley sure, that it's ecs test, but in some way it checks that
 * components are accessible */
TEST_CASE("Physics System updates TransformComponent correctly",
          "[ECS][Physics]") {
  Registry registry;
  EntityID entity = registry.create_entity();

  registry.add_component<TransformComponent>(entity, {{0.0f, 0.0f, 0.0f}});
  registry.add_component<PhysicsComponent>(
      entity, {{1.0f, 0.0f, 0.0f}, {0, 0, 0}, 1.0f});

  float dt = 1.0f;
  PhysicsSystem::update(registry, dt);

  auto &transform = registry.get_component<TransformComponent>(entity);

  REQUIRE(transform.position.x == Catch::Approx(1.0f)); // x += 1 * dt
  REQUIRE(transform.position.y == Catch::Approx(0.0f));
  REQUIRE(transform.position.z == Catch::Approx(0.0f));
}
