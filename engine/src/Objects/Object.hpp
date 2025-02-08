/**
 * @file Object.hpp
 * @author Dario
 * @date 11/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <core.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include "Collision/Collider.hpp"

namespace Sigma {

struct Transform {
  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 offset = glm::vec3(0.0f);
  glm::vec2 scale = glm::vec2(100.0f);
  glm::vec2 relativeScale = glm::vec2(1.0f);
  float rotation = 0.0f;
  float localRotation = 0.0f; ///< @brief This rotation doesn't consider the offset of the pivot

  /**
   * @brief Calculates the matrix of the transform
   * Uses translateLocal * rotateLocal * translateOffset * rotateOffset * scale
   * @return glm::mat3
   */
  [[nodiscard]] glm::mat3 GetMatrix() const {
    //Changed this to use only one matrix -m
    glm::mat3 matrix;
    matrix = glm::translate(matrix, glm::vec2(position.x, position.y));
    matrix = glm::rotate(matrix, localRotation);
    matrix = glm::translate(matrix, glm::vec2(offset.x, offset.y));
    matrix = glm::rotate(matrix, rotation);
    matrix = glm::scale(matrix, scale * relativeScale);
    return matrix;
  }

  /**
   * @brief Calculates a 4x4 Transform matrix
   * This is used in order to have a camera instead of screen coordinates
   * @return glm::mat4
   */
  [[nodiscard]] glm::mat4 GetMatrix4() const {
    // Thy the fuck are Mtx33 and Mtx44 so different -x
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, position);
    matrix = glm::rotate(matrix, localRotation, glm::vec3(0.0f, 0.0f, 1.0f));
    matrix = glm::translate(matrix, offset);
    matrix = glm::rotate(matrix, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    matrix = glm::scale(matrix, glm::vec3(scale.x * relativeScale.x, scale.y * relativeScale.y, 1.0f));
    return matrix;
  }

  /**
   * @brief Returns the pivot world coordinates
   *
   * Insetead of using local coordinates, you can offset the pivot of any object so that it isn't always in the center
   * This is useful for rotating objects from a point different to its center
   *
   * This function will return the world coordinates of the pivot of the object so you can calculate distance between 
   * object pivots for whatever reason you need to do it
   *
   * @return glm::vec3
   */
  [[nodiscard]] glm::vec3 GetPivot() const { return position + offset; }
};

// At least give me credit for copying this from my engine -x

class Event;

namespace Collision {
class CollisionEvent;
}

class Object {
  using EventCallbackFn = std::function<void(Event &)>; ///< Type alias for the event callback function
protected:
  explicit Object(const uint32_t id) : m_id(id) {}

public:
  virtual ~Object() {};

public:
  // copy constructors
  Object(const Object &) = delete;
  Object &operator=(const Object &) = delete;
  Object(Object &&) = default;
  Object &operator=(Object &&) = default;

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
  virtual bool OnMessage(Object *sender) { return false; }
  /**
   * @brief This function is called whenever the object collides with another one
   * @param e The CollisionEvent sent by the Collision System
   * @return A bool telling if the message has been handled and shouldn't propagate
   */
  virtual bool OnCollision(Collision::CollisionEvent &e) { return false; }
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
   *
   * The callback should have been set up by the factory. Example of usage
   * @code
   * // We create a message event that asks for a pointer of this object (using 'this' keyword) and the name of the
   * // object we want to send the event to
   * MessageEvent eventTest(this, "Object2");
   * // Then we use the SendEvent function to send it to the OnEvent buffer
   * SendEvent(testEvent);
   *
   *
   * // On the receiver, we call the @c OnMessage function and we do the logic we want to do whenever we recive
   * // a message
   * // Note that the sender is a pointer to the object that sent the message
   * // The return tells the system if the event has been handled; if not, it will continue to propagate down
   * bool Object2::OnMessage(Object* sender) {
   *   std::cout << "Message received! (from: " << sender->GetName() << ")\n";
   *   return true;
   * }
   * @endcode
   */
  void SendEvent(Event &e) const { m_callback(e); }

  [[nodiscard]] id_t GetId() const { return m_id; }

  [[nodiscard]] std::string GetName() const { return m_name; }
  void SetName(const std::string &name) { m_name = name; }

  [[nodiscard]] bool IsPersistent() const { return m_persistent; }

  [[nodiscard]] bool GetStartHandled() const { return m_startHandled; }
  void SetStartHandled() { m_startHandled = true; }

  [[nodiscard]] Collision::BoxCollider *GetCollider() { return m_collider.get(); }

protected:
  std::unique_ptr<Collision::BoxCollider> m_collider = nullptr;

private:
  id_t m_id = -1;
  std::string m_name = "Object";

  bool m_active = true;
  bool m_persistent = false;
  bool m_startHandled = false;

  EventCallbackFn m_callback = nullptr;
};

typedef std::unordered_map<id_t, Object *> ObjectMap;

} // namespace Sigma

#undef GLM_ENABLE_EXPERIMENTAL
