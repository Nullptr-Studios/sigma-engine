#include "GameManager.hpp"
#include "StateManager.hpp"
#include "Audio/AudioEngine.hpp"
#include "Events/Event.hpp"
#include "Events/MessageEvent.hpp"
#include "Factory.hpp"
#include "Scene.hpp"
#include "Objects/Camera.hpp"

namespace FNFE {

GameManager* GameManager::m_instance = nullptr;
EngineState StateManager::m_currentEngineState = ENGINE_IDLE;

GameManager::GameManager(const char *title, int width, int height)
    : m_title(title), m_width(width), m_height(height) {
  m_instance = this;
  GameInit();
}

GameManager::~GameManager() {
  m_instance = nullptr;
}

void GameManager::Uninitialize() {
  StateManager::SetEngineState(ENGINE_EXIT);
  m_factory->DestroyAllObjects();

  
  AEGfxTriFree(m_factory->GetSharedTriList());

  // The crash was due to LIVEUPDATE
  m_audioEngine->Terminate();
}

void GameManager::GameInit()
{
  StateManager::SetEngineState(ENGINE_INIT);

  // Initialize alpha engine
  AEDbgAssertFunction(AESysInit(m_title, m_width, m_height), __FILE__, __LINE__, "AESysInit() failed!");

  m_factory = std::make_unique<Factory>(this, &GameManager::OnEvent);
  m_factory->FreeAllTextures();
  
  // Initialize audio engine
  m_audioEngine = std::make_unique<AudioEngine>();
  m_audioEngine->Init();

  auto camera = FNFE_FACTORY->CreateObject<Camera>("Main Camera");
  m_activeCamera = camera;

  StateManager::SetEngineState(IN_GAME);
}


void GameManager::Run() {
  // AE Shit
  AESysFrameStart();
  AESysUpdate();

  // Start
  for (const auto& [id, object] : m_factory->GetObjects()) {
    object->Start();
  }

  if (m_currentScene != nullptr)
  {
    // Collisions
    // TODO: For Each Actor UpdateCollisionList(actor.getCollider());
    // TODO: For Each Actor CollideObject(obj,other_obj);
    // Tick
    for (const auto& [id, object] : m_factory->GetObjects()) {
      object->Update(AEGetFrameTime());
    }

    // Render
    for (const auto& [id, renderable] : m_factory->GetRenderables()) {
      AEMtx44 camera = m_activeCamera->GetCameraMatrix();
      AEMtx44 transform = renderable->transform.GetMatrix4();
      AEMtx44 viewMatrix = camera * transform;
      AEGfxSetTransform(&viewMatrix);
      if(renderable->GetTexture() != nullptr)
        AEGfxTextureSet(renderable->GetTexture());
      AEGfxTriDraw(m_factory->GetSharedTriList());
    }
  }
  
  // Audio
  m_audioEngine->Set3DListenerPosition(m_activeCamera->transform.position.x,m_activeCamera->transform.position.y,0,0,1,0,0,0,1);
  m_audioEngine->Update();

  // AE Shit
  AESysFrameEnd();
}

// Scene Management
#pragma region Scene Management

void GameManager::LoadScene(Scene *scene) {
  if (scene == nullptr) {
    std::cout << "[GameManager] Scene to load is nullptr" << std::endl;
    return;
  }

  if (m_currentScene != nullptr) {
    m_currentScene->Free();
    m_currentScene->Unload();

    delete m_currentScene;
  }

  m_currentScene = scene;
  std::cout << "[GameManager] Scene: " << m_currentScene->GetName() << " with ID: " << scene->GetID() << " loading..."
            << std::endl;
  m_currentScene->Load();
  m_currentScene->Init();
  std::cout << "[GameManager] Scene: " << m_currentScene->GetName() << " with ID: " << scene->GetID() << " loaded!"
            << std::endl;
}

#pragma endregion

void GameManager::OnEvent(Event &e) {
  EventDispatcher dispatcher(e);

  for (const auto& [id, object] : m_factory->GetObjects()) {
    dispatcher.Dispatch<MessageEvent>([object](MessageEvent& e)->bool{
      if(object->GetName() == e.GetReceiver()) return object->OnMessage(e.GetSender());
      return false;
    });
  }

}


} 
