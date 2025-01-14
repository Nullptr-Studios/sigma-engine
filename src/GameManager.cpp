

#include "GameManager.hpp"

#include <iostream>

#include <aecore/AEDebug.h>
#include <aecore/AESystem.h>

#include "Audio/AudioEngine.hpp"
#include "Scene.hpp"

namespace FNFE {

GameManager::GameManager(const char *title, int width, int height)
  : m_title(title), m_width(width), m_height(height)
{
  GameInit();
}

GameManager::~GameManager() {}

void GameManager::Uninitialize() { m_audioEngine->Terminate(); }

void GameManager::GameInit()
{
  // Initialize alpha engine
  AEDbgAssertFunction(AESysInit(m_title, m_width, m_height), __FILE__, __LINE__, "AESysInit() failed!");

  m_audioEngine = std::make_unique<AudioEngine>();
  m_audioEngine->Init();

  AudioData data = AudioData("res/illegal4.mp3", true, true, 1.0f);

  m_audioEngine->Load(data);
  //Master bank is needed to load other banks
  m_audioEngine->LoadBank("res/Master.bank");
  m_audioEngine->LoadBank("res/Master.strings.bank");

  //Load the music bank
  m_audioEngine->LoadBank("res/Music.bank");
  m_audioEngine->LoadEvent("event:/Music/OST_Credits");
  m_audioEngine->PlayEvent("event:/Music/OST_Credits");

}


void GameManager::Run()
{
  AESysFrameStart();
  AESysUpdate();
  if (m_currentScene != nullptr)
  {
    m_currentScene->Update(AEGetFrameTime());
    m_currentScene->Draw();
    //@TODO: Implement Factory in order to tick the objects
  }
  m_audioEngine->Set3DListenerPosition(1*AESin(AEGetTime()),1*AECos(AEGetTime()),0,0,1,0,0,0,1);
  m_audioEngine->Update();
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

  if (m_currentScene != nullptr)
  {
    m_currentScene->Free();
    m_currentScene->Unload();

    delete m_currentScene;
  }

  m_currentScene = scene;
  std::cout << "[GameManager] Scene: " << m_currentScene->GetName() << " with ID: " << scene->GetID() << " loading..." << std::endl;
  m_currentScene->Load();
  m_currentScene->Init();
  std::cout << "[GameManager] Scene: " << m_currentScene->GetName() << " with ID: " << scene->GetID() << " loaded!" << std::endl;

}

#pragma endregion

} // FNFE