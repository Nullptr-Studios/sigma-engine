/**
 * @file Factory.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 12/01/2025
 *
 * @brief Handles the creation and destruction of game objects
 */

#pragma once
#include <pch.hpp>

#include "Core.hpp"
#include "Objects/Actor.hpp"
#include "Objects/Object.hpp"

namespace FNFE {

class GameManager;

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

  /**
   * @typedef TextureMap
   * @brief Type used for mapping
   */
  typedef std::unordered_map<std::string, AEGfxTexture*> TextureMap;

public:
  Factory(GameManager* manager, void(GameManager::*callback)(Event &e))
    : m_managerInstance(manager), m_managerCallback(callback) {
    m_instance = this;
  }
  ~Factory();

  // Copy constructor
  Factory &operator=(const Factory &) = delete;
  Factory(const Factory &) = delete;

// Objects
#pragma region Objects

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

  void DestroyAllObjects(); ///< @brief Destroys all objects and clears the object map

  ObjectMap GetObjects() { return m_objects; } ///< @brief Returns the Object map
  ActorMap GetRenderables() { return m_renderables; } ///< @brief Returns the Renderables map

#pragma endregion

// Textures
#pragma region Textures

  /**
   *  @brief Adds texture to the pool
   *  This function checks if the texture is already loaded, if not, it loads it into memory and returns it. If it is
   *  loaded it simply returns the texture
   *
   * @param filepath Filepath of the texture to load
   * @return @c AEGfxTexture* of the texture
   */
  AEGfxTexture * LoadTexture(const char* filepath);
  /**
   * @brief Frees textures from the pool
   * Tells Alpha Engine to free the textures from memory and deletes the entry on the pool
   * @param filepath
   */
  void FreeTexture(const char* filepath);
  void FreeAllTextures(); ///< @brief Frees memory for all textures and clears the texture pool

  TextureMap GetTextures() { return m_textures; } ///< @brief Returns the texture pool

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

  GameManager* m_managerInstance;
  void(GameManager::*m_managerCallback)(Event&);

  /**
   * @brief Map that contains all objects
   *
   * This map contains a @c std::shared_ptr for every object that exist in the game stored by its ID. This map is
   * used to call all the object's functions like @c Update. The Factory class has ownership of all the objects.
   */
  ObjectMap m_objects;
  /**
   * @brief Map that contains all renderable objects
   *
   * This map contains a @c std::shared_ptr for every object (Actor class or inherited) that is renderable in the game
   * stored by its ID. This map os used for rendering objects in game.
   */
  ActorMap m_renderables;
  /**
   * @brief Map that contains all textures
   *
   * This maps contains pointers for every loaded texture currently in the game. The textures have their relative path
   * as a key
   */
  TextureMap m_textures;
};

// Object declaration
template<typename T, typename>
std::shared_ptr<T> Factory::CreateObject(const std::string& name) {
  std::shared_ptr<T> obj = std::make_shared<T>(m_currentId);
  obj->SetName(name);
  // dont even ask about this -x
  obj->SetCallback(std::bind(m_managerCallback, m_managerInstance, std::placeholders::_1));
  m_currentId++;

  // This crashes the program, it will work for now -x
  if (m_currentId == 0) {
    throw std::runtime_error("Object ID overflow");
  }

  m_objects.emplace(obj->GetId(), std::static_pointer_cast<Object>(obj));

  if constexpr (std::is_base_of_v<Actor, T>) {
    m_renderables.emplace(obj->GetId(), std::static_pointer_cast<Actor>(obj));
  }

  std::cout << "[Factory] Created object " << name << " with ID: " << obj->GetId() << "\n";
  return obj;
}

}
