/**
 * @file ControllerComponent.hpp
 * @author dante
 * @date 1/17/2025
 *
 * @brief Abstract Class for player and AI controller
 */
#pragma once

namespace FNFE {
struct Transform;
}
struct AEVec3;
namespace FNFE {

/**
 * @class ControllerComponent Abstract Controller class for PlayerController and AIController to implement
 */
class ControllerComponent {
public:
  ControllerComponent(Transform* transform) : m_transform(transform) {};
  virtual ~ControllerComponent() = default;
  ControllerComponent(const ControllerComponent &) = delete;
  ControllerComponent &operator=(const ControllerComponent &) = delete;
  ControllerComponent(ControllerComponent &&) = default;
  ControllerComponent &operator=(ControllerComponent &&) = default;

  
protected:
  AEVec3 m_position{}; ///<@brief 3D position of the object
  Transform* m_transform{}; ///<@brief Pointer to the transform of the object
  virtual void Update()=0; ///<@brief update function to update the player/ai
};
} // namespace FNFE
