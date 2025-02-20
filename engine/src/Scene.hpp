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

  void AddChild(Object* obj) {
    m_ownedObjects.push_back(obj);
  }

  /**
   * @brief Get the name of this scene
   *
   * @return const char*
   */
  const char *GetName() const { return m_name; }

  /**
   * @brief Get the ID of this scene
   *
   * @return unsigned
   */
  unsigned GetID() const { return m_ID; }

public:
  Sigma::Polygon *m_sceneBoundsPoly{};

private:
  const char *m_name;
  unsigned m_ID;

  std::list<Object *> m_ownedObjects = {};
};

} // namespace Sigma
