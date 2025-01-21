/**
 * @file GameManager.hpp
 * @author Dario
 * @date 11/01/2025
 *
 * @brief AudioEngine
 */

#pragma once
#include <pch.hpp>

namespace FNFE {

class AudioEngine;
class Factory;
class Scene;
class Event;
enum GameState : char;

/**
 * @class GameManager
 * @brief Main engine class
 *
 * This class is in charge of all the engine, it initializes all core components and runs the main loop. This
 * class must own all core components of the engine.
 */
class GameManager {
  friend class StateManager;
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

  void OnEvent(Event& e);

private:
  static GameManager* m_instance;
  static GameState m_gameState;

  void GameInit();

  const char* m_title;
  int m_width;
  int m_height;

  std::unique_ptr<Factory> m_factory;
  std::unique_ptr<AudioEngine> m_audioEngine;

  Scene* m_currentScene = nullptr;
};
}
