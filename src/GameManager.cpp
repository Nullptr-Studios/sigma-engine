/**
 * @file GameManager.cpp
 * @author Dario
 * @date 11/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#include "GameManager.hpp"

#include <iostream>

#include <aecore/AEDebug.h>
#include <aecore/AESystem.h>

#include "Audio/AudioEngine.hpp"
#include "Scene.hpp"

namespace FNFE {

GameManager::GameManager(const char *title, int width, int height)
{
  this->m_title = title;
  this->m_width = width;
  this->m_height = height;

  GameInit();
}

GameManager::~GameManager() {}

void GameManager::GameInit()
{
  // Initialize alpha engine
  AEDbgAssertFunction(AESysInit(m_title, m_width, m_height), "GameManager.cpp", __LINE__, "AESysInit() failed!");

  audioEngine = std::make_unique<AudioEngine>();
  audioEngine->Init();

  AudioData data = AudioData("res/illegal4.mp3", true, true, 1.0f);

  audioEngine->Load(data);
  audioEngine->Play(data);
}


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
  audioEngine->Set3DListenerPosition(1*AESin(AEGetTime()),1*AECos(AEGetTime()),0,0,1,0,0,0,1);
  audioEngine->Update();
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