/**
 * @file Core.hpp
 * @author Xein
 * @date 12/01/2025
 *
 * @brief Core definitions used in all the game engine
 * @note This file is included on the @c pch.hpp precompiled header
 */

#pragma once

namespace FNFE {

typedef unsigned int id_t;     ///< @typedef id_t   @brief Type definition for object IDs
typedef nlohmann::json json_t; ///< @typedef json_t @brief Type definition for JSON files

#define FNFE_FACTORY Factory::GetInstance()     ///< @def FNFE_FACTORY @brief Gets Factory instance
#define FNFE_MANAGER GameManager::GetInstance() ///< @def FNFE_MANAGER @brief Gets Manager instance

/**
 * @enum GameState
 * @brief Holds all the possible states the game could be in
 * 
 * This is useful for disabling input or doing specific things the objects should only do in some states of the game.
 * All levels have been condensed into a single @c IN_GAME state for simplification of if statements. If some code
 * needs to only be run on a certain state, call @code GetCurrentSceneID() @endcode to get the scene that is playing
 * at that moment
 */
enum GameState : char {
  ENGINE_IDLE = 0, ///< @brief Default state
  ENGINE_INIT,     ///< @brief Engine initialization
  LOADING_SCENE,   ///< @brief Loading resources from the scene
  UNLOADING_SCENE, ///< @brief Unloading resources from the scene
  IN_GAME,         ///< @brief Player is playing the game 
  IN_MENU,         ///< @brief Any type of UI Menu is opening
  ENGINE_ENDING,   ///< @breif Engine is closing the game
};

}
