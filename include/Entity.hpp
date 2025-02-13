/**
 * @file Entity.hpp
 * @brief Defines the Entity class, which serves as a wrapper for ECS entities.
 *
 * The Entity class provides a high-level abstraction over the
 * Entity-Component-System (ECS), allowing entities to be created and
 * manipulated without directly interfacing with the Registry.
 */

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Logger.hpp"
#include "Registry.hpp"
#include <iostream>

/**
 * @class Entity
 * @brief A high-level ECS entity wrapper.
 *
 * This class simplifies entity management by automatically handling unique IDs
 * and component operations. It also enforces type safety while working with
 * components.
 *
 * Entities in this system are **lightweight**; they primarily serve as IDs that
 * reference component data stored in the Registry.
 *
 * @note This design ensures that entities are just identifiers (not heavy
 * objects), keeping performance optimal in large-scale simulations.
 */
class Entity {
public:
  /**
   * @brief Constructs an Entity and registers it in the ECS.
   * @param registry Reference to the Registry.
   *
   * When an Entity is created, it is automatically registered with the ECS and
   * assigned a unique ID. No need to assign id_ yourself
   *
   * @note Entities should NOT store data themselves; all states are stored in
   * components.
   */
  Entity(Registry &registry) : registry_(registry) {
    id_ = registry_.create_entity();
  }

  /**
   * @brief Adds a component of type 'T' to the object.
   * @tparam T The component type.
   * @tparam Args Variadic template arguments for the component constructor.
   * @param args Arguments forwarded to the component constructor.
   *
   * If the entity already has this component, the function logs a warning.
   */
  template <typename T, typename... Args> void add_component(Args &&...args) {
    if (registry_.has_component<T>(id_)) {
      LOG(WARNING, "Entity %d already has component %s", id_, typeid(T).name());
      return;
    }
    registry_.add_component<T>(id_, T(std::forward<Args>(args)...));
  }

  /**
   * @brief Removes a component of type 'T' from the object.
   * @tparam T The component type to remove.
   *
   * If the component does not exist, a warning is logged.
   */
  template <typename T> void remove_component() {
    if (!registry_.has_component<T>(id_)) {
      LOG(WARNING, "Entity %d does not have component %s", id_,
          typeid(T).name());
      return;
    }
    registry_.remove_component<T>(id_);
  }

  /**
   * @brief Retrieves a pointer to the specified component type 'T'.
   * @tparam T The component type to retrieve.
   * @return A pointer to the component, or `nullptr` if not found.
   *
   * This function provides kinda safe component access by returning nullptr
   * if the entity does not have the requested component.
   */
  template <typename T> T *get_component() {
    if (!registry_.has_component<T>(id_)) {
      return nullptr; // component doesn't exist
    }
    return &registry_.get_component<T>(id_);
  }

  EntityID id() const { return id_; }

protected:
  EntityID id_;
  Registry &registry_;
};

#endif // !ENTITY_HPP
