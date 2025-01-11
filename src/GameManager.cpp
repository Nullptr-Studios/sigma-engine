/**
 * @file GameManager.cpp
 * @author Dario
 * @date 11/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#include "GameManager.hpp"

#include <aecore/AEAudio.h>
#include <aecore/AEDebug.h>
#include <aecore/AEGameStateMgr.h>
#include <aecore/AESystem.h>
#include <iostream>

#include "Scene.hpp"

namespace FNFE {

GameManager::GameManager(const char *title, int width, int height)
{
  // Initialize alpha engine
  AEDbgAssertFunction(AESysInit(title, width, height), "GameManager.cpp", __LINE__, "AESysInit() failed!");

  AEDbgAssertFunction(AEAudioInitialize(), "GameManager.cpp", __LINE__, "AEAudioInitialize() failed!");

  std::cout << "[GameManager] AE initialized!" << std::endl;

}

GameManager::~GameManager() {}


void GameManager::Run()
{
  AESysFrameStart();
  AESysUpdate();
  if (currentScene != nullptr)
  {
    currentScene->Update(AEGetFrameTime());
    currentScene->Draw();
    //@TODO: Implement Factory in order to tick the objects
  }
  AESysFrameEnd();
}

#pragma region Scene Management

void GameManager::LoadScene(Scene* scene)
{
  if (scene == nullptr)
  {
    std::cout << "[GameManager] Scene to load is nullptr" << std::endl;
    return;
  }

  if (currentScene != nullptr)
  {
    currentScene->Free();
    currentScene->Unload();

    delete currentScene;
  }

  currentScene = scene;
  std::cout << "[GameManager] Scene: " << currentScene->GetName() << " with ID: " << scene->GetID() << " loading..." << std::endl;
  currentScene->Load();
  currentScene->Init();
  std::cout << "[GameManager] Scene: " << currentScene->GetName() << " with ID: " << scene->GetID() << " loaded!" << std::endl;

}

#pragma endregion

} // FNFE