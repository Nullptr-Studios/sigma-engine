/**
 * @file PlayerController.hpp
 * @author dante
 * @date 1/20/2025
 *
 * @brief Manages and interprets input to action
 */
#pragma once
#include "ControllerComponent.hpp"
#include "InputSystem.hpp"
namespace FNFE {
class InputSystem;
}
namespace FNFE {
enum PlayerState {
  NEUTRAL,
  AIR,
  ACTION,
};

class PlayerController final : public ControllerComponent {
public:
  AEVec2 pos{};
  void Update() override;

private:
  void PlayAction(PlayerAction action);
  void EndAction(PlayerAction action);
  int m_controllerId{};
  InputSystem m_inputSystem;
  PlayerState m_state = NEUTRAL;
};
} // namespace FNFE
