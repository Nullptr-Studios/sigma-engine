/**
 * @file main.cpp
 * @author Xein
 * @date 1/23/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#include <aecore/AEEngine.h>
#include <iostream>

#include "GameManager.hpp"
#include "Scene.hpp"
#include "TestScene.hpp"

int main() {

  Sigma::GameManager* gameManager = new Sigma::GameManager("FNF Engine", 800, 600);
  Sigma::Scene* scene = new TestScene("scene1", 1, "res/scene1.json");

  gameManager->LoadScene(scene);

  while (AESysWindowExists()) {
    gameManager->Run();
  }
  
  gameManager->Uninitialize();
  AESysExit();

  return 0;
}
