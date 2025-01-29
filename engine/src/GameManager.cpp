#include "GameManager.hpp"
#include "StateManager.hpp"
#include "Audio/AudioEngine.hpp"
#include "Events/Event.hpp"
#include "Events/MessageEvent.hpp"
#include "Factory.hpp"
#include "Scene.hpp"
#include "AnimationSystem/AnimationSystem.hpp"
#include "Collision/Collider.hpp"
#include "Objects/Camera.hpp"
#include "Collision/Collision.hpp"
#include "GlmAlphaTools.hpp"

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

  PROFILER_START
  StateManager::SetEngineState(ENGINE_EXIT);
  m_factory->DestroyAllObjects();

  m_factory->FreeAllTextures();
  
  AEGfxTriFree(m_factory->GetSharedTriList());

  // The crash was due to LIVEUPDATE -d
  m_audioEngine->Terminate();

  PROFILER_END("GameManager::Uninitialize")
}

void GameManager::GameInit()
{

  PROFILER_START
  
  StateManager::SetEngineState(ENGINE_INIT);

  // Initialize alpha engine
  AEDbgAssertFunction(AESysInit(m_title, m_width, m_height), __FILE__, __LINE__, "AESysInit() failed!");

  // Initialize factory
  m_factory = std::make_unique<Factory>(this, &GameManager::OnEvent);
  
  // Initialize audio engine
  m_audioEngine = std::make_unique<AudioEngine>();
  m_audioEngine->Init();

  m_animationSystem = std::make_unique<ANIMATION::AnimationSystem>();
  m_activeCamera = FNFE_FACTORY->CreateObject<Camera>("Main Camera");

  StateManager::SetEngineState(IN_GAME);


  // Start
  for (const auto& [id, object] : m_factory->GetObjects()) {
    object->Start();
    object->SetStartHandled();
  }

  PROFILER_END("GameManager::GameInit")
}


void GameManager::Run() {

  // AE Shit
  AESysFrameStart();
  AESysUpdate();

  if (m_currentScene != nullptr)
  {

    // TODO: For Each Actor Deubug DrawRectCollider
    auto a = m_factory->GetObjects();
    for (const auto& [id, object] : m_factory->GetObjects()) {
      if (object == nullptr) continue;
      if (!object->GetStartHandled()) {
        object->Start();
        object->SetStartHandled();
      }
      object->Update(AEGetFrameTime());
    }

    m_currentScene->Update(AEGetFrameTime());

    // Render
    for (const auto& renderableId : m_factory->GetRenderables()) {
      auto actor = dynamic_cast<Actor*>(m_factory->GetObjectAt(renderableId));
      if (!actor->GetStartHandled()) continue; // We do this because the object has not had its Start method done yet
      
      AEMtx44 camera = m_activeCamera->GetCameraMatrix();
      AEMtx44 transform = actor->transform.GetMatrix4();
      AEMtx44 viewMatrix = camera * transform;
      AEGfxSetTransform(&viewMatrix);
      AEGfxTextureSet(actor->GetTexture());
      auto textureTransform = glm::ToAEX(actor->GetTextureTransform());
      AEGfxSetTextureTransform(&textureTransform);
      AEGfxTriDraw(m_factory->GetSharedTriList());
    }

    m_currentScene->Draw();
    
  }

  auto textures = m_factory->GetTextures();
  
  // Audio
  m_audioEngine->Set3DListenerPosition(m_activeCamera->transform.position.x,m_activeCamera->transform.position.y,0,0,1,0,0,0,1);
  m_audioEngine->Update();

#if _DEBUG

  if (m_debug) {
    std::string loadedTextures = "Loaded Textures: ";
    loadedTextures.append(std::to_string(AEGfxGetAllocatedTexturesCount()));
    AEGfxPrint(10, 10, 0xFFFFFFFF, loadedTextures.c_str());

    std::string loadedTriLists = "Loaded Tri Lists: ";
    loadedTriLists.append(std::to_string(AEGfxGetAllocatedTrilistCount()));
    AEGfxPrint(10, 20, 0xFFFFFFFF, loadedTriLists.c_str());

    std::string loadedFonts = "Loaded Fonts: ";
    loadedFonts.append(std::to_string(AEGfxGetAllocatedFontCount()));
    AEGfxPrint(10, 30, 0xFFFFFFFF, loadedFonts.c_str());

    std::string FPS = "FPS: ";
    FPS.append(std::to_string(AEGetFrameRate()));
    AEGfxPrint(600, 10, 0xFFFFFFFF, FPS.c_str());
    
    std::string SPF = "SPF: ";
    SPF.append(std::to_string(AEGetFrameTime()));
    AEGfxPrint(600, 20, 0xFFFFFFFF, SPF.c_str());
  }
#endif
  
  // AE Shit
  AESysFrameEnd();

}

// Scene Management
#pragma region Scene Management

void GameManager::LoadScene(Scene *scene) {

  PROFILER_START
  
  if (scene == nullptr) {
    std::cout << "[GameManager] Scene to load is nullptr" << std::endl;
    return;
  }

  if (m_currentScene != nullptr) {
    m_currentScene->Free();
    m_currentScene->Unload();

    m_factory->DestroyAllObjects(); 

    delete m_currentScene;
  }

  m_currentScene = scene;
  std::cout << "[GameManager] Scene: " << m_currentScene->GetName() << " with ID: " << scene->GetID() << " loading..."
            << std::endl;
  m_currentScene->Load();
  m_currentScene->Init();
  std::cout << "[GameManager] Scene: " << m_currentScene->GetName() << " with ID: " << scene->GetID() << " loaded!"
            << std::endl;

  PROFILER_END("GameManager::LoadScene")
}

#pragma endregion

void GameManager::OnEvent(Event &e) {

  PROFILER_START
  
  EventDispatcher dispatcher(e);

  for (const auto& [id, object] : m_factory->GetObjects()) {
    dispatcher.Dispatch<MessageEvent>([object](MessageEvent& e)->bool{
      if(object->GetName() == e.GetReceiver()) return object->OnMessage(e.GetSender());
      return false;
    });
  }

  PROFILER_END("GameManager::OnEvent")

}


} 
