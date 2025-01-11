/**
 * @file Object.hpp
 * @author Dario
 * @date 11/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include <aecore/AEVec2.h>
#include <aecore/AEVec3.h>
#include <string>

namespace FNFE {

struct Transform {
  AEVec3 position = {};
  AEVec2 scale = {};
  float rotation = 0.0f;
};

class Object {
protected:
  explicit Object(const uint32_t id) : m_id(id) {}
  virtual ~Object() = default;

public:
  //copy constructors
  Object(const Object&) = delete;
  Object& operator=(const Object&) = delete;
  Object(Object&&) = default;
  Object& operator=(Object&&) = default;

  Transform transform;


  virtual void Init() {}
  virtual void Start() {}
  virtual void Update(double delta) {}
  virtual void Draw() {}
  virtual void Destroy() {}


  uint32_t GetId() const { return m_id; }
  std::string GetName() const { return m_name; }

  void SetName(const std::string& name) { m_name = name; }

  bool IsPersistent() const { return m_persistent; }

private:

  uint32_t m_id = 0;
  std::string m_name = "Base Object";

  bool m_active = true;
  bool m_persistent = false;

};

} // FNFE
