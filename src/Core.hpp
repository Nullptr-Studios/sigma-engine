/**
 * @file Core.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 12/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once

namespace FNFE {

typedef unsigned int id_t;     ///< @typedef id_t   @brief Type definition for object IDs
typedef nlohmann::json json_t; ///< @typedef json_t @brief Type definition for json files

#define FNFE_FACTORY Factory::GetInstance() ///< @def FNFE_FACTORY @brief Gets Factory instance
#define FNFE_MANAGER GameManager::GetInstance() ///< @def FNFE_MANAGER @brief Gets Manager instance

/**
 * @enum GameState
 * @brief Stores all possible game states
 * This is useful for disabling input on menus, handling the Begin method of objects...
 */
enum GameState : unsigned char {
  ENGINE_IDLE = 0, ///< @brief The engine is not in any state (default)
  ENGINE_INIT,     ///< @brief The engine is initializing
  SCENE_LOAD,      ///< @brief A scene is currently being loaded
  IN_GAME,         ///< @brief The engine is running a scene
  IN_MENU,         ///< @brief The game is stopped and a menu/UI is shown
  SCENE_UNLOAD,    ///< @brief A scene is currently being unloaded
  ENGINE_EXIT,     ///< @brief The engine is stopping all its processes
};

}
