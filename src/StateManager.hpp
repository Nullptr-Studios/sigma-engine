/**
 * @file StateManager.hpp
 * @author Xein
 * @date 1/21/2025
 *
 * @brief Handles all Game State stuff
 */

#pragma once

namespace FNFE {

enum GameState : char;

/**
 * @class StateManager
 * @brief Interfaces with the @c GameManager to get the current state the game is in
 *
 * This is made like this to avoid bidirectionally including the GameManager and things like objects or the factory.
 * This class should be expanded in the future
 */
class StateManager {
  friend class GameManager;
public:
  static GameState GetCurrent(); ///< @brief Gets the current Game State @return The Game State enum
  static void SetCurrent(GameState state); ///< @brief Changes the current Game State @param state State to change to
};

}
