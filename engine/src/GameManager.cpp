#include "GameManager.hpp"
#include "AnimationSystem/AnimationSystem.hpp"
#include "Audio/AudioEngine.hpp"
#include "Collision/Collider.hpp"
#include "Collision/Collision.hpp"
#include "Collision/CollisionEvent.hpp"
#include "Controller/CameraController.hpp"
#include "Events/Event.hpp"
#include "Events/MessageEvent.hpp"
#include "Factory.hpp"
#include "GlmAlphaTools.hpp"
#include "Objects/Camera.hpp"
#include "Scene.hpp"
#include "StateManager.hpp"

namespace Sigma {

GameManager *GameManager::m_instance = nullptr;
EngineState StateManager::m_currentEngineState = ENGINE_IDLE;

GameManager::GameManager(const char *title, int width, int height) : m_title(title), m_width(width), m_height(height) {
  m_instance = this;
  GameInit();
}

GameManager::~GameManager() { m_instance = nullptr; }
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

void GameManager::GameInit() {

  PROFILER_START

  StateManager::SetEngineState(ENGINE_INIT);

  // Initialize alpha engine
  AEDbgAssertFunction(AESysInit(m_title, m_width, m_height), __FILE__, __LINE__, "AESysInit() failed!");

  // disable depth buffer cuz were using our own, fuck alpha btw
  AEGfxSetDepthBufferEnabled(false);

  // Initialize factory
  m_factory = std::make_unique<Factory>(this, &GameManager::OnEvent);

  // Initialize audio engine
  m_audioEngine = std::make_unique<AudioEngine>();
  m_audioEngine->Init();

  m_collisionSystem = std::make_unique<Collision::CollisionSystem>([this](Event& e)
  {
    this->OnEvent(e);
  });

  m_animationSystem = std::make_unique<ANIMATION::AnimationSystem>();
  m_cameraController = std::make_unique<Sigma::CameraController>(0); // idk if this is fine
  m_cameraController->SetCurrentCamera(GET_FACTORY->CreateObject<Camera>("Main Camera"));
  StateManager::SetEngineState(IN_GAME);

#ifdef _DEBUG
 
  // Disables 60fps lock in Debug mode
  AESetFrameRateMax(20000); 
  
#endif // _DEBUG

  // Start
  for (const auto &object: *m_factory->GetObjects() | std::views::values) {
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
#if _DEBUG
    auto startCollision = std::chrono::high_resolution_clock::now();
#endif
    m_collisionSystem->UpdateCollisions(m_factory->GetObjects());
#if _DEBUG
    auto endCollision = std::chrono::high_resolution_clock::now();
    m_timeCollisions = endCollision - startCollision;
#endif
    
    
    // TODO: For Each Actor Deubug DrawRectCollider

    
#if _DEBUG
    auto startTick = std::chrono::high_resolution_clock::now();
#endif
    // Scene and subscene update
    m_currentScene->Update(AEGetFrameTime());

    for (auto &val: m_subScenes | std::views::values) {
      val->Update(AEGetFrameTime());
    }

    // Tick Objects
    for (const auto &object: *m_factory->GetObjects() | std::views::values) {
      if (object == nullptr)
        continue;
      if (!object->GetStartHandled()) {
        object->Start();
        object->SetStartHandled();
      }
      object->Update(AEGetFrameTime());
    }

#if _DEBUG
    auto endTick = std::chrono::high_resolution_clock::now();
    m_timeTick = endTick - startTick;
#endif

    
#if _DEBUG
    auto startDraw = std::chrono::high_resolution_clock::now();
#endif
    // Scene and SubScene draw
    m_currentScene->Draw();

    for (auto &val: m_subScenes | std::views::values) {
      val->Draw();
    }

    //Sort by Z order
    m_factory->GetRenderables()->sort([](const id_t& a, const id_t& b)
    {
      const auto OA = GET_FACTORY->GetObjectAt(a);
      const auto OB = GET_FACTORY->GetObjectAt(b);

      return OA->transform.position.z < OB->transform.position.z;
    });
    
    // Render Objects
    for (const auto &renderableId: *m_factory->GetRenderables()) {
      
      auto actor = dynamic_cast<Actor *>(m_factory->GetObjectAt(renderableId));
      if (!actor->GetStartHandled())
        continue; // We do this because the object has not had its Start method done yet

      glm::mat4 world = actor->transform.GetMatrix4();
      auto worldAE = ToAEX(world);
      AEGfxSetTransform(&worldAE);
      auto viewAE = AEMtx44::Identity();
      AEGfxSetViewTransform(&viewAE);
      glm::mat4 proj = m_cameraController->GetCurrentCamera()->GetCameraMatrix();
      auto projAE = ToAEX(proj);
      AEGfxSetProjTransform(&projAE);

      AEGfxTextureSet(actor->GetTexture());
      auto textureTransform = glm::ToAEX(actor->GetTextureTransform());
      AEGfxSetTextureTransform(&textureTransform);
      AEGfxTriDraw(m_factory->GetSharedTriList());
    }

#if _DEBUG
    auto endDraw = std::chrono::high_resolution_clock::now();
    m_timeRender = endDraw - startDraw;
#endif

  }

#if _DEBUG
    auto startSound = std::chrono::high_resolution_clock::now();
#endif
  
  // Audio
  m_audioEngine->Set3DListenerPosition(m_cameraController->GetCurrentCamera()->transform.position.x, m_cameraController->GetCurrentCamera()->transform.position.y, 0, 0,
                                       1, 0, 0, 0, 1);
  m_audioEngine->Update();

  
#if _DEBUG
    auto endSound = std::chrono::high_resolution_clock::now();
    m_timeSound = endSound - startSound;
#endif

  DebugProfiler();
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

    // TODO: Do Scene object ownership
    m_factory->DestroyAllObjects();

    for (auto &val: m_subScenes | std::views::values) {
      val->Free();
      val->Unload();
      delete val;
    }

    delete m_currentScene;
  }

  m_currentScene = scene;
  std::cout << "[GameManager] Scene: " << m_currentScene->GetName() << " with ID: " << m_currentScene->GetID() << " loading..."
            << std::endl;
  m_currentScene->Load();
  m_currentScene->Init();
  std::cout << "[GameManager] Scene: " << m_currentScene->GetName() << " with ID: " << m_currentScene->GetID() << " loaded!"
            << std::endl;

  PROFILER_END("GameManager::LoadScene")
}
void GameManager::LoadSubScene(Scene *scene) {

  PROFILER_START

  if (m_subScenes.contains(scene->GetID())) {
    std::cout << "[GameManager]: SubScene already loaded \n";
    return;
  }

  scene->Load();
  scene->Init();

  m_subScenes.emplace(scene->GetID(), scene);

  std::cout << "[GameManager] Scene: " << scene->GetName() << " with ID: " << scene->GetID() << " loaded as a SubScene!"
            << std::endl;

  PROFILER_END("GameManager::LoadSubScene")
}
void GameManager::UnloadSubScene(Scene *scene) {
  if (m_subScenes.contains(scene->GetID())) {
    scene->Free();
    scene->Unload();
    m_subScenes.erase(scene->GetID());
    delete scene;
    return;
  }
  std::cout << "[GameManager] Scene: " << scene->GetName() << " with ID: " << scene->GetID()
            << " not found in SubScenes!" << std::endl;
}
void GameManager::UnloadSubScene(const int id) {
  if (m_subScenes.contains(id)) {
    Scene *scene = m_subScenes[id];
    scene->Free();
    scene->Unload();
    m_subScenes.erase(id);
    delete scene;
    return;
  }
  std::cout << "[GameManager] Scene with ID: " << id << " not found in SubScenes!" << std::endl;
}

#pragma endregion

void GameManager::OnEvent(Event &e) {

  PROFILER_START

  EventDispatcher dispatcher(e);

  dispatcher.Dispatch<Collision::CollisionEvent>([](Collision::CollisionEvent& collision)->bool
    {
      auto obj = GET_FACTORY->GetObjectAt(collision.GetReceiver());
      if (obj) return obj->OnCollision(collision);

      return false;
    });

  for (const auto &object: *m_factory->GetObjects() | std::views::values) {
    dispatcher.Dispatch<MessageEvent>(
        [object](MessageEvent &e) -> bool
        {
          if (object->GetName() == e.GetReceiver())
            return object->OnMessage(e.GetSender());
          return false;
        });
  }

  PROFILER_END("GameManager::OnEvent")
}
void GameManager::DebugProfiler()
{
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
    float fps = AEGetFrameRate();
    
    unsigned colorFPS = 0xFFFFFFFF;
    if (fps >= 59) {
      colorFPS = 0xFF00FF00;
    } else if (fps >= 29) {
      colorFPS = 0xFFFFFF00;
    } else {
      colorFPS = 0xFFFF0000;
    }
    
    FPS.append(std::to_string(fps));
    AEGfxPrint(600, 10, colorFPS, FPS.c_str());
    
    std::string SPF = "SPF: ";
    SPF.append(std::to_string(AEGetFrameTime()));
    AEGfxPrint(600, 20, colorFPS, SPF.c_str());

    std::string Collisions = "COL: ";
    Collisions.append(std::to_string(m_timeCollisions.count()));
    AEGfxPrint(600, 35, 0xFF00FF00, Collisions.c_str());

    std::string Tick = "TCK: ";
    Tick.append(std::to_string(m_timeTick.count()));
    AEGfxPrint(600, 45, 0xFF00FF00, Tick.c_str());

    std::string Draw = "DRW: ";
    Draw.append(std::to_string(m_timeRender.count()));
    AEGfxPrint(600, 55, 0xFF00FF00, Draw.c_str());

    std::string Sound = "SND: ";
    Sound.append(std::to_string(m_timeSound.count()));
    AEGfxPrint(600, 65, 0xFF00FF00, Sound.c_str());
    

    std::string CurrentObjects = "Current Objects: ";
    CurrentObjects.append(std::to_string(m_factory->GetObjects()->size()));
    AEGfxPrint(10, 50, 0xFFFFFFFF, CurrentObjects.c_str());

    std::string CurrentActors = "Current Actors: ";
    CurrentActors.append(std::to_string(m_factory->GetRenderables()->size()));
    AEGfxPrint(10, 60, 0xFFFFFFFF, CurrentActors.c_str());

    std::string CurrentObjectsList = "Current Objects List: \n";
    for (auto &val: *m_factory->GetObjects() | std::views::values) {
      CurrentObjectsList.append(val->GetName());
      CurrentObjectsList.append("\n");
    }
    CurrentObjectsList.append(std::to_string(m_factory->GetRenderables()->size()));
    AEGfxPrint(10, 80, 0xFFFFFFFF, CurrentObjectsList.c_str());
  }
#endif
}

} // namespace Sigma
