/**
 * @file Scene.hpp
 * @author Dario
 * @date 11/01/2025
 *
 */

#pragma once
#include <list>


#include "Factory.hpp"
#include "Polygon.hpp"
#include "core.hpp"


namespace Sigma {
class Object;

class Scene {
public:
  /**
   * @brief Construct a new Scene object
   *
   * @param name
   * @param ID
   */
  Scene(const char *name, unsigned ID) {
    m_name = name;
    m_ID = ID;
  }

  virtual ~Scene() {};

  virtual void Load() {};
  virtual void Init() {}
  virtual void Update(double delta) {}
  virtual void Draw() {}
  virtual void Free() {}
  virtual void Unload() {
    for (auto object: m_ownedObjects) {
      GET_FACTORY->DestroyObject(object);
    }
    m_ownedObjects.clear();
  }

  virtual void DebugWindow() {
    ImGui::Text("Scene: %s", m_name);
    ImGui::Text("ID: %i", m_ID);
  }

  
  /**
   * @brief Set the ownership of an object to this scene
   *
   * @param obj
   */
  void AddChild(Object* obj) 
  {
      //Sanity fucking check (I spent like 1h cuz i was adding the player two times to the array, and it was crashing somewhere completely different) -d
    if (obj == nullptr)
      return;

    for (auto object: m_ownedObjects) {
      if (object == obj)
        return;
    }

    m_ownedObjects.push_back(obj);
  }

  /**
   * @brief Get the name of this scene
   *
   * @return const char*
   */
  [[nodiscard]] const char *GetName() const { return m_name; }

  /**
   * @brief Get the ID of this scene
   *
   * @return unsigned
   */
  [[nodiscard]] unsigned GetID() const { return m_ID; }

public:
  Sigma::Polygon *m_sceneBoundsPoly{};

private:
  const char *m_name;
  unsigned m_ID;

  std::list<Object *> m_ownedObjects = {};
};

} // namespace Sigma
