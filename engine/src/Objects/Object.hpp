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
  [[nodiscard]] AEMtx33& GetMatrix() const {
    //Changed this to use only one matrix -m
    AEMtx33 world;
    AEMtx33ScaleApply(&world,&world, scale.x, scale.y);
    AEMtx33RotApply(&world, &world, rotation);
    AEMtx33TransApply(&world, &world, position.x, position.y);

    return world;
  }

  /**
   * @brief Calculates a 4x4 Transform matrix
   * This is used in order to have a camera instead of screen coordinates
   * @return AEMtx44
   */
  [[nodiscard]] AEMtx44& GetMatrix4() const {
    // Thy the fuck are Mtx33 and Mtx44 so different -x
    AEMtx44 world = AEMtx44::Identity();
    AEMtx44 scaleMat, rotateMat, translateMat;
    scaleMat = AEMtx44::Scale(scale.x, scale.y, 1.0f);
    rotateMat = AEMtx44::Rotate(0.0f, 0.0f, rotation);
    translateMat = AEMtx44::Translate(position.x, position.y, 0.0f);
    world = translateMat * rotateMat * scaleMat;
    return world;
  }
};

// At least give me credit for copying this from my engine -x

class Event;

class Object {
  using EventCallbackFn = std::function<void(Event&)>; ///< Type alias for the event callback function
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

  /**
   * @brief This function is called whenever you send a message to any Object
   * The simple and incredible FNF Event System (tm) is used for sending messages between objects
   * @param sender Object that sent the original message
   * @return A bool telling if the message has been handled and shouldn't propagate
   */
  virtual bool OnMessage(Object* sender) { return false; }
  /**
   * @brief This function sets up the callback for the events
   * This is used when setting up the object, and it's needed for the events to propagate to other objects
   * @warning USE THIS ONLY WHEN SETTING UP OBJECTS
   * @param function Function to call when Message sent
   */
  void SetCallback(const EventCallbackFn &function) { m_callback = function; }
  /**
   * @brief Sends an @c Event to the specified callback function
   * @param e Event to send
   */
  void SendEvent(Event& e) const { m_callback(e); }

  [[nodiscard]] id_t GetId() const { return m_id; }
  [[nodiscard]] std::string GetName() const { return m_name; }

  void SetName(const std::string& name) { m_name = name; }

  [[nodiscard]] bool IsPersistent() const { return m_persistent; }

private:
  id_t m_id = -1;
  std::string m_name = "Object";

  bool m_active = true;
  bool m_persistent = false;

  EventCallbackFn m_callback = nullptr;

};

typedef std::unordered_map<id_t, std::shared_ptr<Object>> ObjectMap;

} // FNFE
