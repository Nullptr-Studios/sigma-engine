#include <aecore/AEEngine.h>
#include <iostream>

#include "GameManager.hpp"
#include "Scene.hpp"
#include "TestScene.hpp"


int main() {

  FNFE::GameManager* gameManager = new FNFE::GameManager("FNF Engine", 800, 600);
  FNFE::Scene* scene = new FNFE::Scene("scene1", 1);

  gameManager->LoadScene(scene);

  while (AESysWindowExists())
  {
    gameManager->Run();
  }
  gameManager->Uninitialize();
  AESysExit();



  return 0;
}
