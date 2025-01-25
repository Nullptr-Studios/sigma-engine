#include "GameManager.hpp"
#include "StateManager.hpp"
#include "Audio/AudioEngine.hpp"
#include "Events/Event.hpp"
#include "Events/MessageEvent.hpp"
#include "Factory.hpp"
#include "Scene.hpp"
#include "Collision/Collider.hpp"
#include "Objects/Camera.hpp"
#include "Collision/Collision.hpp"

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

  m_factory->FreeAllTextures();
  
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
  //m_factory->FreeAllTextures();
  
  // Initialize audio engine
  m_audioEngine = std::make_unique<AudioEngine>();
  m_audioEngine->Init();

  auto camera = FNFE_FACTORY->CreateObject<Camera>("Main Camera");
  m_activeCamera = camera;

  StateManager::SetEngineState(IN_GAME);

  // Start
  for (const auto& [id, object] : m_factory->GetObjects()) {
    object->Start();
    object->SetStartHandled();
  }
}


void GameManager::Run() {
  // AE Shit
  AESysFrameStart();
  AESysUpdate();

  if (m_currentScene != nullptr)
  {
        // Collisions
    // For Each Actor UpdateCollisionList(actor.getCollider());
auto renderable = m_factory->GetRenderables();

for (const auto& renderableId : renderable) {
  auto updateActor = dynamic_cast<Actor*>(m_factory->GetObjectAt(renderableId));
  if (!updateActor->IsInViewport())
    continue;
  UpdateCollisionList(updateActor->GetCollider());
}
   // For Each Actor CollideObject(obj,other_obj);
    auto renderableA = renderable.begin();
    int k2 = 0;
  for (int i = 0; i < renderable.size(); i++) {
    id_t& idA = *renderableA;
    auto actorA = dynamic_cast<Actor*>(m_factory->GetObjectAt(idA));
    if (!actorA->IsInViewport()) {

      if (renderableA != renderable.end()) {
        
        k2++;
      std::advance(renderableA, 1);
      continue;
      }
      else {
        break;
      }
    }
    auto renderableB = renderable.begin();
    for (int k = k2;k < renderable.size(); k++) {
      id_t& idB = *renderableB;
      auto actorB = dynamic_cast<Actor*>(m_factory->GetObjectAt(idB));
    if (!actorB->IsInViewport()) {
      if (renderableB != renderable.end()) {
        
        std::advance(renderableB, 1);
        continue;
      }else {
        break;
      }
    }
    if (actorA != actorB) {
      Collision::CollideObject(actorA, actorB);

      // std::cout << Collision::CollideObject(actorA, actorB);
      // Collision::DrawRectCollider(actorA, AE_COLORS_BLUE);
      // Collision::DrawRectCollider(actorB, AE_COLORS_BLUE);
    }
      if (renderableB != renderable.end()) {
        std::advance(renderableB, 1);

      }else {
        break;
      }

  }
    if (renderableA != renderable.end()) {
      k2++;
      std::advance(renderableA, 1); 

    }else {
      break;
    }
}
#ifndef NDEBUG

#endif
    // TODO: For Each Actor Deubug DrawRectCollider
    auto a = m_factory->GetObjects();
    for (const auto& [id, object] : m_factory->GetObjects()) {
      if (!object->GetStartHandled()) {
        object->Start();
        object->SetStartHandled();
      }
      object->Update(AEGetFrameTime());
    }

    // Render
    for (const auto& renderableId : m_factory->GetRenderables()) {
      auto actor = dynamic_cast<Actor*>(m_factory->GetObjectAt(renderableId));
      if (!actor->GetStartHandled()) continue; // We do this because the object has not had its Start method done yet
      
      AEMtx44 camera = m_activeCamera->GetCameraMatrix();
      AEMtx44 transform = actor->transform.GetMatrix4();
      AEMtx44 viewMatrix = camera * transform;
      AEGfxSetTransform(&viewMatrix);
      AEGfxTextureSet(actor->GetTexture());
      AEGfxSetTextureTransform(actor->GetTextureTransform());
      AEGfxTriDraw(m_factory->GetSharedTriList());
    }
  }

  auto textures = m_factory->GetTextures();
  
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
