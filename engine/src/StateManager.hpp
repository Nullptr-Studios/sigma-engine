/**
 * @file StateManager.hpp
 * @author Xein
 * @date 1/21/2025
 *
 * @brief Handles the state of the game in any moment
 */

#pragma once

namespace sigma {

enum EngineState : unsigned char; 

/**
 * @class StateManager
 * @brief Handles the current state of the game
 */
class StateManager {
public:
  static EngineState GetEngineState() { return m_currentEngineState; } ///< @brief Gets the current state @return The EngineState enum
  static void SetEngineState(const EngineState state) { m_currentEngineState = state; } ///< @brief Updates the current state @param state New engine state
  
private:
  static EngineState m_currentEngineState;
};

}
