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
#include "StateManager.hpp"

namespace Sigma {

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

  typedef std::list<id_t> ActorList;

  /**
   * @typedef TextureMap
   * @brief Type used for mapping
   */
  typedef std::unordered_map<std::string, AEGfxTexture*> TextureMap;

public:
  Factory(GameManager* manager, void(GameManager::*callback)(Event &e))
    : m_managerInstance(manager), m_managerCallback(callback) {
    m_instance = this;
    InitializeTriList();
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
   * @return Pointer of the created object
   */
  template <typename T, typename = std::enable_if_t<std::is_base_of_v<Object, T>>>
  T* CreateObject(const std::string& name = "Unnamed Object");

  /**
   * Destroys an object by its ID
   * @param id Object ID
   */
  void DestroyObject(id_t id);

  /**
   * Destroys an object by its reference
   * @param object Object reference
   */
  void DestroyObject(const Object * object);

  void DestroyAllObjects(); ///< @brief Destroys all objects and clears the object map

  ObjectMap* GetObjects() { return &m_objects; } ///< @brief Returns the Object map
  Object* GetObjectAt(id_t id); ///< @brief Returns an object by ID
  Object* FindObject(const std::string& name); ///< @brief Find an object by its name @warn DO NOT ABUSE
  
  ActorList* GetRenderables() { return &m_renderables; } ///< @brief Returns the Renderables map

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

  AEGfxTriList* GetSharedTriList() { return m_tris; } ///< @brief Returns the shared TriList

  static Factory* GetInstance() {
    if (m_instance == nullptr) {
      throw std::runtime_error("Factory instance not created");
    }
    return m_instance;
  }
    
private:

  bool m_log = false;

  id_t m_currentId = 0;
  static Factory* m_instance;

  GameManager* m_managerInstance;
  void(GameManager::*m_managerCallback)(Event&);

  /**
   * @brief TriList for rendering
   *
   * This is a pointer to the TriList that will be used to render all renderables in the game.
   */
  AEGfxTriList* m_tris = nullptr;

  void InitializeTriList(); ///< @brief Initializes the TriList

  /**
   * @brief Map that contains all objects
   *
   * This map contains a @c std::shared_ptr for every object that exist in the game stored by its ID. This map is
   * used to call all the object's functions like @c Update. The Factory class has ownership of all the objects.
   */
  ObjectMap m_objects;
  /**
   * @brief List that contains all renderable objects
   *
   * This list contains the @c id_t of every renderable object (@c Actor) so they can be called on the draw call
   */
  ActorList m_renderables;
  /**
   * @brief Map that contains all textures
   *
   * This maps contains pointers for every loaded texture currently in the game. The textures have their relative path
   * as a key
   */
  TextureMap m_textures;
};

template<typename T, typename>
T* Factory::CreateObject(const std::string& name) {
  T* obj = new T(m_currentId);
  obj->SetName(name);
  // dont even ask about this -x
  obj->SetCallback(std::bind(m_managerCallback, m_managerInstance, std::placeholders::_1));
  m_currentId++;

  // This crashes the program, it will work for now -x
  if (m_currentId == 0) {
    throw std::runtime_error("Object ID overflow");
  }

  m_objects.emplace(obj->GetId(), dynamic_cast<Object*>(obj));

  if constexpr (std::is_base_of_v<Actor, T>) m_renderables.emplace_back(obj->GetId());
  
  if (m_log)
    std::cout << "[Factory] Created object " << name << " with ID: " << obj->GetId() << "\n";

  obj->Init();

  // If the game is already on the Game Loop (the player is on a level or a menu), we call the Start method on creation
  // If not, we will call it when every object is called at the Invoke Begin phase -x
  // if (StateManager::GetEngineState() == IN_GAME) obj->Start();
  
  return obj;
}

}
