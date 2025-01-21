/**
 * @file ControllerComponent.hpp
 * @author dante
 * @date 1/17/2025
 *
 * @brief Abstract Class for player and AI controller
 */
#pragma once

struct AEVec3;
namespace FNFE {

/**
 * @class ControllerComponent Abstract Controller class for PlayerController and AIController to implement
 */
class ControllerComponent {
public:
  ControllerComponent() = default;
  virtual ~ControllerComponent() = default;
  // Though Shall Not Copy
  ControllerComponent(const ControllerComponent &) = delete;
  ControllerComponent &operator=(const ControllerComponent &) = delete;
  ControllerComponent(ControllerComponent &&) = default;
  ControllerComponent &operator=(ControllerComponent &&) = default;

protected:
  AEVec3 position{};
  
  virtual void Update()=0;
};
} // namespace FNFE
