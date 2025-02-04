/**
 * @file GameManager.hpp
 * @author Dario
 * @date 11/01/2025
 *
 * @brief AudioEngine
 */

#pragma once
#include <Factory.hpp>
#include <pch.hpp>

namespace Sigma {

class CameraController;
class Camera;
class AudioEngine;
class Scene;
class Event;

namespace ANIMATION {
class AnimationSystem;
}

namespace Collision {
class CollisionSystem;
}


/**
 * @class GameManager
 * @brief Main engine class
 *
 * This class is in charge of all the engine, it initializes all core components and runs the main loop. This
 * class must own all core components of the engine.
 */
class GameManager
{
public:
  /**
   * @brief Initialization of alpha engine and base sigma classes
   *
   * @param title window title
   * @param width window width
   * @param height window height
   */
  GameManager(const char* title, int width, int height);
  ~GameManager();

  static GameManager* GetInstance() { return m_instance; }


  /**
   * @brief Main engine loop
   */
  void Run();

  /**
   * @brief Uninitializes the engine
   */
  void Uninitialize();

  /**
   * @brief Loads a scene and unloads the currently loaded scene
   *
   * @param scene scene to load
   */
  void LoadScene(Scene *scene);

  void LoadSubScene(Scene *scene);

  void UnloadSubScene(Scene* scene);

  void UnloadSubScene(int id);

  Scene *GetCurrentScene() { return m_currentScene; }

  void OnEvent(Event &e);

private:

  void DebugProfiler();

  bool m_debug = true;
  
  static GameManager* m_instance;

  void GameInit();

  const char* m_title;
  int m_width;
  int m_height;

  std::unique_ptr<Factory> m_factory;
  std::unique_ptr<AudioEngine> m_audioEngine;
  std::unique_ptr<ANIMATION::AnimationSystem> m_animationSystem;
  std::unique_ptr<Collision::CollisionSystem> m_collisionSystem;
  std::unique_ptr<CameraController> m_cameraController;

  Scene *m_currentScene = nullptr;
  std::unordered_map<int, Scene *> m_subScenes;
};

} // Sigma
