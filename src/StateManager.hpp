/**
 * @file StateManager.hpp
 * @author Xein
 * @date 1/21/2025
 *
 * @brief Handles the state of the game in any moment
 */

#pragma once

namespace FNFE {

enum GameState : unsigned char; 

/**
 * @class StateManager
 * @brief Handles the current state of the game
 */
class StateManager {
public:
  static GameState GetGameState() { return m_currentState; } ///< @brief Gets the current state @return The GameState enum
  static void SetGameState(const GameState state) { m_currentState = state; } ///< @brief Updates the current state @param state New game state
  
private:
  static GameState m_currentState;
};

}
