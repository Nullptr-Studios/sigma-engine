/**
 * @file Core.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 12/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include <json.hpp>
#include <pch.hpp>

namespace Sigma {

typedef unsigned int id_t;     ///< @typedef id_t     @brief Type definition for object IDs
typedef nlohmann::json json_t; ///< @typedef json_t   @brief Type definition for json files
typedef unsigned unsigmad;     ///< @typedef unsigmad @brief Unsigned type def for the Sigma engine
typedef unsigned color_t;      ///< @typedef color_t  @brief Color type for Alpha Engine

#define GET_MANAGER Sigma::GameManager::GetInstance() ///< @def GET_MANAGER @brief Gets Manager instance
#define GET_FACTORY Sigma::Factory::GetInstance()     ///< @def GET_FACTORY @brief Gets Factory instance
#define GET_ANIMATION Sigma::Animation::AnimationSystem::GetInstance() ///< @def GET_ANIMATION @brief Gets Animation instance
#define GET_AUDIO GET_MANAGER->GetAudioEngine() ///< @def GET_AUDIO @brief Gets Audio instance

#define GET_CAMERA Sigma::CameraController::GetCameraControllerInstance() ///< @def GET_CAMERA @brief Gets Camera instance
#define GET_SCENE GET_MANAGER->GetCurrentScene() ///< @def GET_SCENE @brief Gets Current Scene
#define GET_CAMERA Sigma::CameraController::GetCameraControllerInstance() ///< @def GET_CAMERA @brief Gets Camera Controller instance

// profiler
#define PROFILER_START auto start = std::chrono::high_resolution_clock::now();
#define PROFILER_END(FunctionName) auto end = std::chrono::high_resolution_clock::now();\
std::chrono::duration<double> elapsed = end - start;\
std::cout << "[Profiler] [" << (FunctionName) << "] Elapsed time " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << " ms\n";

/**
 * @enum EngineState
 * @brief Stores all possible game states
 * This is useful for disabling input on menus, handling the Begin method of objects...
 */
enum EngineState : unsigned char {
  ENGINE_IDLE = 0, ///< @brief The engine is not in any state (default)
  ENGINE_INIT,     ///< @brief The engine is initializing
  SCENE_LOAD,      ///< @brief A scene is currently being loaded
  IN_GAME,         ///< @brief The engine has loaded everything and is running a scene
  SCENE_UNLOAD,    ///< @brief A scene is currently being unloaded
  ENGINE_EXIT,     ///< @brief The engine is stopping all its processes
};

// TODO: Do GameState enum

}
