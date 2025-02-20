#include "GameManager.hpp"
#include "AnimationSystem/AnimationSystem.hpp"
#include "Audio/AudioEngine.hpp"
#include "Collision/Collision.hpp"
#include "Collision/CollisionEvent.hpp"
#include "Controller/CameraController.hpp"
#include "DamageSystem/DamageEvent.hpp"
#include "Events/Event.hpp"
#include "Events/MessageEvent.hpp"
#include "Factory.hpp"
#include "GlmAlphaTools.hpp"
#include "Objects/Camera.hpp"
#include "Objects/Character.hpp"
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

#pragma region GameLoop
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

  // Initialize Collisions
  m_collisionSystem = std::make_unique<Collision::CollisionSystem>([this](Event& e)
  {
    this->OnEvent(e);
  });

  // Initialize Animation system
  m_animationSystem = std::make_unique<Animation::AnimationSystem>();

  // Initialize camera controller
  m_cameraController = GET_FACTORY->CreateObject<CameraController>("Camera Controller");
  //m_cameraController->SetCurrentCamera(GET_FACTORY->CreateObject<Camera>("Main Camera"));
  StateManager::SetEngineState(IN_GAME);


#ifdef _DEBUG

  // Disables 60fps lock in Debug mode
  AESetFrameRateMax(10000);

#endif // _DEBUG

  // Start all objects
  for (const auto &object: *m_factory->GetObjects() | std::views::values) {
    object->Start();
    object->SetStartHandled();
  }

  // End of initialization
  PROFILER_END("GameManager::GameInit")
}

void GameManager::Run() {

  // AE Shit
  AESysFrameStart();
  AESysUpdate();

    
#if _DEBUG
    auto startCollision = std::chrono::high_resolution_clock::now();
#endif
    // Collision update
    m_collisionSystem->UpdateCollisions(m_factory->GetObjects());
    
#if _DEBUG
    auto endCollision = std::chrono::high_resolution_clock::now();
    m_timeCollisions = endCollision - startCollision;
#endif


    // TODO: For Each Actor Debug DrawRectCollider

#if _DEBUG
    auto startTick = std::chrono::high_resolution_clock::now();
#endif
    // Scene and subscene update
    for (auto scene: m_loadedScenes) {
      scene->Update(AEGetFrameTimeClamped());
    }


    // Tick Objects
    for (const auto &object: *m_factory->GetObjects() | std::views::values) {
      if (!object->GetStartHandled()) {
        object->Start();
        object->SetStartHandled();
      }else {
        // tick after one frame from start
        object->Update(AEGetFrameTimeClamped());
      }
    }

#if _DEBUG
    auto endTick = std::chrono::high_resolution_clock::now();
    m_timeTick = endTick - startTick;
#endif


#if _DEBUG
    auto startDraw = std::chrono::high_resolution_clock::now();
#endif
    // Scene and SubScene draw
    for (auto scene: m_loadedScenes) {
      scene->Draw();
    }



    auto renderables = m_factory->GetRenderables();

    // Do sorting each 5 frames
    if (AEGetFrameCounter() % 5 == 0) {
      // Sort by Z order
      renderables->sort(
          [](const id_t &a, const id_t &b)
          {
            const auto OA = GET_FACTORY->GetObjectAt(a);
            const auto OB = GET_FACTORY->GetObjectAt(b);

            return OA->transform.position.z < OB->transform.position.z;
          });
    }

    // Render Objects
    for (const auto &renderableId: *renderables) {

      auto actor = dynamic_cast<Actor *>(m_factory->GetObjectAt(renderableId));

      // culling
      /*if (!actor->IsInViewport())
        continue;*/

      if (!actor->GetStartHandled())
        continue; // We do this because the object has not had its Start method done yet

      // call the actor draw function (we were not using the draw method until now xdddd) -d
      actor->Draw();

      glm::mat4 world = actor->transform.GetMatrix4();
      // cameraMatrices[0] correspond to viewSpace and cameraMatrices[1] correspond to clipSpace
      auto cameraMatrices = m_cameraController->GetCurrentCamera()->GetCameraMatrix();
      glm::mat4 matrix = cameraMatrices[1] * cameraMatrices[0] * world;
      auto matrixAE = glm::ToAEX(matrix);
      AEGfxSetTransform(&matrixAE);

      // This is here to avoid alpha engine doing weird shit (hopefully) -x
      // If you see this comment that means my weird idea worked -x
      auto viewAE = AEMtx44::Identity();
      AEGfxSetViewTransform(&viewAE);
      auto projAE = AEMtx44::Identity();
      AEGfxSetProjTransform(&projAE);

      // TODO: ModulationColor not working???? -d
      AEGfxTextureSet(actor->GetTexture());
      auto textureTransform = glm::ToAEX(*actor->GetTextureTransform());
      AEGfxSetTextureTransform(&textureTransform);
      AEGfxSetModulationColor(actor->GetTintAEX());

      auto mesh = actor->GetMesh() ? actor->GetMesh() : m_factory->GetSharedTriList();
      AEGfxTriDraw(mesh);
    }

#if _DEBUG
    auto endDraw = std::chrono::high_resolution_clock::now();
    m_timeRender = endDraw - startDraw;
#endif

    


    for (auto scene: m_scenesToUnload) {
      scene->Free();
      scene->Unload();
      m_loadedScenes.remove(scene);
      std::cout << "[GameManager] Scene with ID: " << scene->GetID() << " unloaded" << std::endl;
      delete scene;
    }
    m_scenesToUnload.clear();

  
    //fush destroyed objects
    m_factory->FlushDestroyQueue();

    // Load scenes
    for (auto scene: m_scenesToLoad) {
      
      m_loadedScenes.push_back(scene);

      std::cout << "[GameManager] Scene: " << scene->GetName() << " with ID: " << scene->GetID()
                << " loading..." << std::endl;
      // Call member functions
      scene->Load();
      scene->Init();
  
      std::cout << "[GameManager] Scene: " << scene->GetName() << " with ID: " << scene->GetID()
                << " loaded!" << std::endl;
      
    }
  m_scenesToLoad.clear();

#if _DEBUG
  auto startSound = std::chrono::high_resolution_clock::now();
#endif

  // Audio update
  m_audioEngine->Set3DListenerPosition(m_cameraController->GetCurrentCamera()->transform.position.x,
                                       m_cameraController->GetCurrentCamera()->transform.position.y, 0, 0, 1, 0, 0, 0,
                                       1);
  m_audioEngine->Update();

#if _DEBUG
  auto endSound = std::chrono::high_resolution_clock::now();
  m_timeSound = endSound - startSound;
#endif

  // Debug Profiler
  DebugProfiler();
  
  // AE Shit
  AESysFrameEnd();
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

void GameManager::OnEvent(Event &e) {

  EventDispatcher dispatcher(e);

  dispatcher.Dispatch<Damage::DamageEvent>([](Damage::DamageEvent & damage)->bool
    {
      auto obj = GET_FACTORY->GetObjectAt(damage.GetReceiver());
      if (const auto dmg = dynamic_cast<Damageable*>(obj)) dmg->OnDamage(damage);

      return true;
    });

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

}
#pragma endregion

#pragma region Scene Management

void GameManager::LoadScene(Scene *scene) {

  if (scene == nullptr) {
    std::cout << "[GameManager] Scene to load is nullptr" << std::endl;
    return;
  }
 
  //Check for duplicate scene
  for (const auto &s: m_loadedScenes)
  {
    if (scene->GetName() == s->GetName() && scene->GetID() == s->GetID())
    {
      std::cout << "[GameManager] Scene: " << scene->GetName() << " with ID: " << scene->GetID() << " already loaded" << std::endl;
      return;
    }
  }

  m_scenesToLoad.push_back(scene);
 
}

void GameManager::UnloadScene(const char *sceneName)
{
  for (const auto scene: m_loadedScenes) {
    if (scene->GetName() == sceneName) {
      UnloadScene(scene->GetID());
      return;
    }
  }
  std::cout << "[GameManager] Scene with name: " << sceneName << " not found" << std::endl;
}

void GameManager::UnloadScene(unsigned sceneID)
{
  PROFILER_START
  
  for (const auto element: m_loadedScenes){
    if (element->GetID() == sceneID) {
      m_scenesToUnload.push_back(element);
      return;
    }
  }

  std::cout << "[GameManager] Scene with ID: " << sceneID << " not found" << std::endl;
}

Scene *GameManager::GetCurrentScene(int ID) {
  for (auto scene: m_loadedScenes) {
    if (scene->GetID() == ID) {
      return scene;
    }
  }
  std::cout << "[GameManager] Scene with ID: " << ID << " not found" << std::endl;
  return nullptr;
}

Scene *GameManager::GetCurrentScene(const char *name) {
  for (auto scene : m_loadedScenes) {
    if (scene->GetName() == name) {
      return scene;
    }
  }
  std::cout << "[GameManager] Scene with name: " << name << " not found" << std::endl;
  return nullptr;
}

#pragma endregion

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
    AEGfxPrint(AEGetWindowSize().x - 255, 10, colorFPS, FPS.c_str());

    std::string SPF = "SPF: ";
    SPF.append(std::to_string(AEGetFrameTimeClamped()));
    AEGfxPrint(AEGetWindowSize().x - 255, 20, colorFPS, SPF.c_str());

    std::string Collisions = "COL: ";
    Collisions.append(std::to_string(m_timeCollisions.count()));
    AEGfxPrint(AEGetWindowSize().x - 255, 35, 0xFF00FF00, Collisions.c_str());

    std::string Tick = "TCK: ";
    Tick.append(std::to_string(m_timeTick.count()));
    AEGfxPrint(AEGetWindowSize().x - 255, 45, 0xFF00FF00, Tick.c_str());

    std::string Draw = "DRW: ";
    Draw.append(std::to_string(m_timeRender.count()));
    AEGfxPrint(AEGetWindowSize().x - 255, 55, 0xFF00FF00, Draw.c_str());

    std::string Sound = "SND: ";
    Sound.append(std::to_string(m_timeSound.count()));
    AEGfxPrint(AEGetWindowSize().x - 255, 65, 0xFF00FF00, Sound.c_str());



    auto mouse = AEGetMouseData();
    glm::vec2 mousePos = {mouse.position.x, mouse.position.y};
    mousePos = GET_CAMERA->GetCurrentCamera()->ScreenToWorld(mousePos);
    std::string mousePosStr = "Mouse Pos: " + std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y);
    AEGfxPrint(AEGetWindowSize().x - 255, 95, 0xFFFFFFFF, mousePosStr.c_str());


    std::string CurrentScenes = "Current loaded Scenes: " + std::to_string(m_loadedScenes.size()) + "\n";

    for (auto scenes: m_loadedScenes) {
      CurrentScenes.append(scenes->GetName());
      CurrentScenes.append(" : ");
      CurrentScenes.append(std::to_string(scenes->GetID()));
      CurrentScenes.append("\n");
    }
    AEGfxPrint(AEGetWindowSize().x - 255, 110, 0xFFFFFFFF, CurrentScenes.c_str());


    std::string CurrentObjects = "Current Objects: ";
    CurrentObjects.append(std::to_string(m_factory->GetObjects()->size()));
    AEGfxPrint(10, 50, 0xFFFFFFFF, CurrentObjects.c_str());

    std::string CurrentActors = "Current Actors: ";
    CurrentActors.append(std::to_string(m_factory->GetRenderables()->size()));
    AEGfxPrint(10, 60, 0xFFFFFFFF, CurrentActors.c_str());

    std::string CurrentObjectsList = "Current Objects List: \n";
    for (auto val: *m_factory->GetObjects() | std::views::values) {
      CurrentObjectsList.append(val->GetName());
      CurrentObjectsList.append("\n");
    }
    CurrentObjectsList.append(std::to_string(m_factory->GetRenderables()->size()));
    AEGfxPrint(10, 80, 0xFFFFFFFF, CurrentObjectsList.c_str());
  }
#endif
}

}
