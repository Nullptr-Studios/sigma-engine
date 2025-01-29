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

namespace FNFE::ANIMATION {
class AnimationSystem;
}

namespace FNFE {
class Camera;
}
namespace FNFE {

class AudioEngine;
class Scene;
class Event;

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
   * @brief Initialization of alpha engine and base FNFE classes
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
  void LoadScene(Scene* scene);

  Scene* GetCurrentScene() { return m_currentScene; }

  void OnEvent(Event& e);

  // TODO: Create a camera controller at some point
  void ChangeCamera(Camera* camera) { m_activeCamera = camera; }

  Camera* GetActiveCamera() { return m_activeCamera; }
private:

  bool m_debug = true;
  
  static GameManager* m_instance;

  void GameInit();

  const char* m_title;
  int m_width;
  int m_height;

  std::unique_ptr<Factory> m_factory;
  std::unique_ptr<AudioEngine> m_audioEngine;
  std::unique_ptr<ANIMATION::AnimationSystem> m_animationSystem;

  Scene* m_currentScene = nullptr;

  Camera* m_activeCamera;
};

} // FNFE
