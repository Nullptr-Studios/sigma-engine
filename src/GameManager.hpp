/**
 * @file GameManager.hpp
 * @author Dario
 * @date 11/01/2025
 *
 * @brief AudioEngine
 */

#pragma once
#include <memory>


namespace FNFE {

class AudioEngine;
class Scene;

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


private:

  void GameInit();

  const char* m_title;
  int m_width;
  int m_height;

  std::unique_ptr<AudioEngine> audioEngine;

  Scene* currentScene = nullptr;
};

} // FNFE
