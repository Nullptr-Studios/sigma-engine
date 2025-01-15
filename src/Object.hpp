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
#include <memory>
#include <string>

#include "Core.hpp"

namespace FNFE {

struct Transform {
  AEVec3 position = AEVec3(0.0f);
  AEVec2 scale = AEVec2(100.0f);
  float rotation = 0.0f;

  /**
   * @brief Calculates the matrix of the transform
   * Uses translate * rotate * scale
   * @return AEMtx33
   */
  AEMtx33& GetMatrix() {
    //Changed this to use only one matrix -m
    AEMtx33 world;
    AEMtx33ScaleApply(&world,&world, scale.x, scale.y);
    AEMtx33RotApply(&world, &world, rotation);
    AEMtx33TransApply(&world, &world, position.x, position.y);

    return world;
  }
};

// At least give me credit for copying this from my engine -x

class Object {
protected:
  explicit Object(const uint32_t id) : m_id(id) { Init(); }
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
  virtual void Update(double deltaTime) {}
  virtual void Draw() {}
  virtual void Destroy() {}


  id_t GetId() const { return m_id; }
  std::string GetName() const { return m_name; }

  void SetName(const std::string& name) { m_name = name; }

  bool IsPersistent() const { return m_persistent; }

private:
  id_t m_id = -1;
  std::string m_name = "Object";

  bool m_active = true;
  bool m_persistent = false;

};

typedef std::unordered_map<id_t, std::shared_ptr<Object>> ObjectMap;

} // FNFE
