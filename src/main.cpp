#include <aecore/AEEngine.h>
#include <iostream>

#include "GameManager.hpp"
#include "Scene.hpp"
#include "TestScene.hpp"

// Test XML include
#include "Audio/AudioEngine.hpp"
#include "rapidxml/rapidxml.hpp"

int main() {

  FNFE::GameManager* gameManager = new FNFE::GameManager("shitty engine", 800, 600);
  FNFE::Scene* scene = new FNFE::Scene("scene1", 1);

  gameManager->LoadScene(scene);

  FNFE::Scene *scene2 = new TestScene("scene2", 2);

  //scene is being deleted inside this LoadScene
  gameManager->LoadScene(scene2);

  while (AESysWindowExists())
  {
    gameManager->Run();
  }

  return 0;
}
