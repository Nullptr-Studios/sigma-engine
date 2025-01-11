/**
 * @file GameManager.hpp
 * @author Dario
 * @date 11/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once

namespace FNFE {

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
   * @brief Loads a scene and unloads the currently loaded scene
   *
   * @param scene scene to load
   */
  void LoadScene(Scene* scene);


private:

  Scene* currentScene = nullptr;
};

} // FNFE
