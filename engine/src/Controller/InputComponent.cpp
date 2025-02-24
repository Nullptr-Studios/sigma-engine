#include "GlmAlphaTools.hpp"
#include "InputComponent.hpp"

namespace Sigma {

InputComponent::InputComponent(const std::string &keybindPath) {

  m_inputBuffer = {};

  std::ifstream file(keybindPath);
  if (!file.is_open()) {
    std::cout << "[InputSystem] failed to open JSON file " << keybindPath << '\n';
    return;
  }

  // Parse json file
  auto keybinds = nlohmann::json::parse(file);

  // Debug log
  std::cout << keybinds << std::endl;
  std::string moveStick = keybinds["gamepad"]["sticks"]["movement"];

  m_movementStick = moveStick != "left";

  auto kMovement = keybinds["keyboard"]["movement"];
  auto kActions = keybinds["keyboard"]["actions"];
  auto gActions = keybinds["gamepad"]["action"];

  for (auto &item: kMovement.items()) {
    std::pair<std::string, std::string> pair = std::make_pair(item.key(), item.value());
    m_keyboardMovement.insert(pair);
  }

  for (auto &item: kActions.items()) {
    std::pair<std::string, std::string> pair = std::make_pair(item.key(), item.value());
    m_keyboardActions.insert(pair);
  }

  for (auto &item: gActions.items()) {
    std::pair<std::string, std::string> pair = std::make_pair(item.key(), item.value());
    m_gamepadActions.insert(pair);
  }

  keybinds.clear();
  
  file.close();
}

void InputComponent::UpdateInput(int controllerId) {
  // Use gamepad stick or keyboard keys for player movement
  UpdateDirection(controllerId);
  // Use Gamepad buttons vs keyboard keys for player actions
  UpdateActions(controllerId);
}

void InputComponent::UpdateDirection(int controllerId) {
  m_movementBuffer = {};
  if (controllerId == -1) {
    if (AEInputKeyPressed(m_keyboardMovement["up"][0]))
      m_movementBuffer.y += 1;
    if (AEInputKeyPressed(m_keyboardMovement["left"][0]))
      m_movementBuffer.x -= 1;
    if (AEInputKeyPressed(m_keyboardMovement["down"][0]))
      m_movementBuffer.y -= 1;
    if (AEInputKeyPressed(m_keyboardMovement["right"][0]))
      m_movementBuffer.x += 1;
  } else {
    if (m_movementStick == 0) { // if left stick
      auto input = AEInputGamepadStickLeft(controllerId);
      m_movementBuffer = glm::FromAEX(input);
    } else { // if right stick
      auto input = AEInputGamepadStickRight(controllerId);
      m_movementBuffer = glm::FromAEX(input);
    }
  }

  if (m_movementBuffer.x != 0)
    m_lastMovementBuffer.x = m_movementBuffer.x;

  if (m_movementBuffer.y != 0)
    m_lastMovementBuffer.y = m_movementBuffer.y;
}

void InputComponent::UpdateActions(int controllerId) {
  if (controllerId == -1) {
    for (auto &action: m_keyboardActions) {
      if (AEInputKeyTriggered(action.second[0])) {
        if (action.first != m_inputBuffer) {
          m_timeBuffer = time(nullptr);
        }
        m_inputBuffer = action.first;
        // std::cout << action.first << std::endl;
        return;
      }
    }
  } else {
    for (auto &action: m_gamepadActions) {
      if (AEInputGamepadButtonTriggered(controllerId, ToGamepadKey(action.second[0]))) {
        m_inputBuffer = action.first;
        m_timeBuffer = time(nullptr);
        return;
      }
    }
  }
  if (!m_inputBuffer.empty() && (time(nullptr) - m_timeBuffer) > .001f) {
    std::cout << "InputBuffer Timeout\n";
    m_inputBuffer.clear();
  }
}

std::string InputComponent::GetAction() {
  std::string tmp = m_inputBuffer;
  m_inputBuffer.clear();
  return tmp;
  
}

int ToGamepadKey(char button) {
  switch (button) {
    case 'X':
      return AE_GAMEPAD_X;
    case 'Y':
      return AE_GAMEPAD_Y;
    case 'A':
      return AE_GAMEPAD_A;
    case 'B':
      return AE_GAMEPAD_B;
    default:
      return '?';
  }
}

int InputComponent::CheckControllers() {
  for (int i = 0; i <= 3; i++) {
    if (AEInputGamepadConnected(i)) {
      return i;
    }
  }
  return -1;
}

} // namespace Sigmauwu
