/**
 * @file Registry.hpp
 * @brief Defines the Registry class for managing ECS entities and components.
 *
 * The 'Registry' class is the core of the ECS (Entity-Component-System).
 * It manages:
 * - Entity creation and destruction.
 * - Component storage and retrieval for each entity.
 *
 * It uses a bitset for component tracking, allowing O(1) filtering
 * of entities with specific components, as it can be critical for
 * high-performance simulations.
 */

#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include <algorithm>
#include <bitset>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>

/// The maximum number of different component types that can exist in the ECS.
/// This allows fast bitset-based component lookups.
constexpr int MAX_COMPONENTS = 32;

// Each entity must have unique ID; Why not store whole objects? Well, it looks
// like storing only int IDs is way more efficient
using EntityID = int;

/**
 * @class IComponentStorage
 * @brief Interface for generic component storage.
 */
class IComponentStorage {
public:
  virtual ~IComponentStorage() = default;
};

/**
 * @class ComponentStorage
 * @tparam T Component type.
 * @brief Stores a specific type of component for entities.
 *
 * Provides O(1) access time for retrieving a component
 * (If I can count. Honestly I have no idea what I'm doing :D)
 */
template <typename T> class ComponentStorage : public IComponentStorage {
private:
  /// Stores component data, mapped to an entity ID.
  std::unordered_map<EntityID, T> components;

public:
  /**
   * @brief Adds a component to an entity.
   * @param entity The entity ID.
   * @param component The component instance to store.
   */
  void add(EntityID entity, const T &component) {
    components[entity] = component;
  }

  /**
   * @brief Removes a component from an entity.
   * @param entity The entity ID.
   */
  void remove(EntityID entity) { components.erase(entity); }

  /**
   * @brief Checks if an entity has this component.
   * @param entity The entity ID.
   * @return True if the entity has the component.
   */
  bool has(EntityID entity) const {
    return components.find(entity) != components.end();
  }
  /**
   * @brief Retrieves a reference to an entity's component.
   * @param entity The entity ID.
   * @return Reference to the component.
   * @throws std::out_of_range if the component does not exist.
   */
  T &get(EntityID entity) { return components.at(entity); }
};

/**
 * @class Registry
 * @brief Manages all ECS entities and components.
 *
 * This class serves as the centralized storage for entities and their
 * associated components.
 *
 * The system is generic and type-safe (Allows any component type to
 * be registered dynamically)
 */
class Registry {
private:
  /// Active entities
  std::vector<EntityID> entities;

  /// Storage pools for each component type (mapped by type hash).
  std::unordered_map<size_t, std::unique_ptr<IComponentStorage>> componentPools;

  /// Bitmask representation of which components each entity has.
  std::unordered_map<EntityID, std::bitset<MAX_COMPONENTS>>
      entityComponentMasks;

  // As entities are stored as integers, this is fast way to make them "unique"
  int nextEntityID = 0;

public:
  /**
   * @brief Creates a new entity and assigns it a unique ID.
   * @return New entity ID.
   */
  EntityID create_entity() {
    EntityID entity = nextEntityID++;
    entities.push_back(entity);
    return entity;
  }

  /**
   * @brief Destroys an entity and removes all its components.
   * @param entity The entity ID to destroy.
   */
  void destroy_entity(EntityID entity) {
    entities.erase(std::remove(entities.begin(), entities.end(), entity),
                   entities.end());
    entityComponentMasks.erase(entity);
  }

  /**
   * @brief Adds a component of type 'T' to an entity.
   * @tparam T The component type.
   * @param entity The entity ID.
   * @param component The component instance to store.
   */
  template <typename T>
  void add_component(EntityID entity, const T &component) {
    // Why hash? Just faster than type_index. Like O(log n) vs O(1), but there
    // are might be hash collisions, not critical though, just restart sim.
    size_t typeID = typeid(T).hash_code();

    // If this is the first time this component type is used, create a storage
    // bucket
    if (componentPools.find(typeID) == componentPools.end()) {
      componentPools[typeID] = std::make_unique<ComponentStorage<T>>();
    }

    // Store component in the bucket
    static_cast<ComponentStorage<T> *>(componentPools[typeID].get())
        ->add(entity, component);

    // Update entity's bitmask
    entityComponentMasks[entity].set(typeID % MAX_COMPONENTS);
  }

  /**
   * @brief Checks if an entity has a component of type 'T'.
   * @tparam T The component type.
   * @param entity The entity ID.
   * @return True if the entity has the component.
   */
  template <typename T> bool has_component(EntityID entity) {
    size_t typeID = typeid(T).hash_code();
    return entityComponentMasks[entity].test(typeID % MAX_COMPONENTS);
  }

  /**
   * @brief Retrieves a reference to a component of type 'T' for an entity.
   * @tparam T The component type.
   * @param entity The entity ID.
   * @return Reference to the component.
   * @throws std::runtime_error if the component does not exist.
   */
  template <typename T> T &get_component(EntityID entity) {
    size_t typeID = typeid(T).hash_code();
    if (componentPools.find(typeID) == componentPools.end()) {
      throw std::runtime_error("Component not found in registry!");
    }
    return static_cast<ComponentStorage<T> *>(componentPools[typeID].get())
        ->get(entity);
  }

  /**
   * @brief Removes a component of type 'T' from an entity.
   * @tparam T The component type.
   * @param entity The entity ID.
   */
  template <typename T> void remove_component(EntityID entity) {
    size_t typeID = typeid(T).hash_code();

    if (componentPools.find(typeID) != componentPools.end()) {
      static_cast<ComponentStorage<T> *>(componentPools[typeID].get())
          ->remove(entity);
      entityComponentMasks[entity].reset(typeID % MAX_COMPONENTS);
    }
  }

  /**
   * @brief Retrieves all entities that have a specific component.
   * @tparam T The component type.
   * @return A vector of entity IDs that have the component.
   *
   * The key optimization here is that I can filter entities in O(1) using
   * ComponentMask, saves a lot of time on cycles, but amount of entities must
   * be specified before simulation starts (before compilation? not sure)
   */
  template <typename T> std::vector<EntityID> get_entities_with_component() {
    std::vector<EntityID> result;
    size_t typeID = typeid(T).hash_code();
    for (auto &[entity, mask] : entityComponentMasks) {
      if (mask.test(typeID % MAX_COMPONENTS)) {
        result.push_back(entity);
      }
    }

    return result;
  }
};

#endif // !REGISTRY_HPP
