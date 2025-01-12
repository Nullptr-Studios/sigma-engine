/**
 * @file Factory.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 12/01/2025
 *
 * @brief Handles the creation and destruction of game objects
 */

#pragma once
#include <iostream>
#include <memory>
#include <stdexcept>
#include "Core.hpp"
#include "GameObject.hpp"
#include "Object.hpp"

namespace FNFE {

/**
 * @class Factory
 * @brief Handles the creation and destruction of game objects
 *
 * This class is in charge of creating, storing and deleting all game objects ever created in the game.
 * By using this class, we can easily manage the game objects and their lifecycles.
 *
 * @note This class is a singleton
 *
 * @note This class has ownership of all game objects
 */
class Factory {
public:
  Factory() { m_instance = this; }
  ~Factory() { m_instance = nullptr; }

  // Copy constructor
  Factory &operator=(const Factory &) = delete;
  Factory(const Factory &) = delete;

#pragma region Factory

  /**
   * Handles the creation of a new object
   *
   * @tparam T Object type
   * @param name Object name
   * @return Shared pointer of the created object
   */
  template <typename T, typename = std::enable_if_t<std::is_base_of_v<Object, T>>>
  std::shared_ptr<T> CreateObject(const std::string& name = "Unnamed Object");

  /**
   * Destroys an object by its ID
   * @param id Object ID
   */
  void DestroyObject(id_t id);

  /**
   * Destroys all objects and clears the object map
   */
  void DestroyAllObjects();

  ObjectMap GetObjects() { return m_objects; } ///< @brief Returns the Object map
  // RenderableMap GetRenderables() { return m_renderables; }

#pragma endregion

  static Factory* GetInstance() {
    if (m_instance == nullptr) {
      throw std::runtime_error("Factory instance not created");
    }
    return m_instance;
  }
    
private:
  id_t m_currentId = 0;
  static Factory* m_instance;

  ObjectMap m_objects;
        
};

template<typename T, typename>
std::shared_ptr<T> Factory::CreateObject(const std::string& name) {
  std::shared_ptr<T> obj = std::make_shared<T>(m_currentId);
  obj->SetName(name);
  m_currentId++;

  // This crashes the program, it will work for now -x
  if (m_currentId == 0) {
    throw std::runtime_error("Object ID overflow");
  }

  m_objects.emplace(obj->GetID(), std::static_pointer_cast<Object>(obj));

  // This is here if we want a list of the objects that are renderable
  // if constexpr (std::is_base_of_v<Actor, T>) {
  //   m_renderables.emplace(obj->GetID(), std::static_pointer_cast<Actor>(obj));
  // }

  std::cout << "Created object " << name << " with ID: " << obj->GetID() << std::endl;
  return obj;
}

}
