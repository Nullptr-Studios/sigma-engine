#include "GameManager.hpp"

#include "Audio/AudioEngine.hpp"
#include "Factory.hpp"
#include "Scene.hpp"

namespace FNFE {

GameManager::GameManager(const char *title, int width, int height)
  : m_title(title), m_width(width), m_height(height) {
  GameInit();
}

GameManager::~GameManager() = default;

void GameManager::Uninitialize()
{
  m_factory->DestroyAllObjects();

  //TODO something crashes here :( -m
  m_audioEngine->Terminate();
}

void GameManager::GameInit()
{
  m_factory = std::make_unique<Factory>();
  m_factory->FreeAllTextures();

  // Initialize alpha engine
  AEDbgAssertFunction(AESysInit(m_title, m_width, m_height), __FILE__, __LINE__, "AESysInit() failed!");

  // Initialize audio engine
  m_audioEngine = std::make_unique<AudioEngine>();
  m_audioEngine->Init();
  {
    //AudioData data = AudioData("res/illegal4.mp3", true, true, 1.0f);

    //m_audioEngine->Load(data);
    //Master bank is needed to load other banks
    m_audioEngine->LoadBank("res/Master.bank");
    m_audioEngine->LoadBank("res/Master.strings.bank");

    //Load the music bank
    m_audioEngine->LoadBank("res/Music.bank");
    std::vector<std::pair<const char*, float>> params;
    params.emplace_back("LowPass", 1);
    m_audioEngine->LoadEvent("event:/Music/OST_Level3", params);
    m_audioEngine->PlayEvent("event:/Music/OST_Level3");
  }

  auto test = FNFE_FACTORY->CreateObject<Actor>("Fucking square");
  
  test->SetTexture("res/toast.png");
}


void GameManager::Run()
{
  // AE Shit
  AESysFrameStart();
  AESysUpdate();

  // Camera
  AEMtx33 cameraMatrix = AEMtx33::IDENTITY;

  if (m_currentScene != nullptr)
  {
    // Tick
    for (const auto& [id, object] : m_factory->GetObjects()) {
      object->Update(AEGetFrameTime());
      object->transform.rotation -= 1 * AEGetFrameTime();
    }

    // Render
    for (const auto& [id, renderable] : m_factory->GetRenderables()) {
      AEMtx33 viewMatrix = renderable->transform.GetMatrix();
      AEGfxSetTransform(&viewMatrix);
      if(renderable->GetTexture() != nullptr) AEGfxTextureSet(renderable->GetTexture());
      AEGfxTriDraw(renderable->GetTris());
    }
  }

  m_audioEngine->SetEventParamValue("event:/Music/OST_Level3", "LowPass", AESin(AEGetTime()));

  // Audio
  m_audioEngine->Set3DListenerPosition(1*AESin(AEGetTime()),1*AECos(AEGetTime()),0,0,1,0,0,0,1);
  m_audioEngine->Update();

  // AE Shit
  AESysFrameEnd();
}

// Scene Management
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