/**
 * @file TestScene.cpp
 * @author Dario
 * @date 11/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#include "TestScene.hpp"
#include "GlmAlphaTools.hpp"
#include "aecore/AEEngine.h"
#include <iostream>

void TestScene::Load() {
  Scene::Load();
  std::cout << "TestScene::Load()" << std::endl;

  glm::mat4 glmMatrix = {1, 2, 3, 4, 5, 6, 7,8, 9, 10, 11, 12, 13, 14, 15, 16};
  AEMtx44 alphaMatrix = {1, 2, 3, 4, 5, 6, 7,8, 9, 10, 11, 12, 13, 14, 15, 16};

  glm::ToAEX(glmMatrix).Print();
  alphaMatrix.Print();
  
}
void TestScene::Update(double delta) {
  Scene::Update(delta);
  std::cout << "TestScene::Update(" << delta << ")" << std::endl;
}
void TestScene::Draw() {
  Scene::Draw();
  std::cout << "TestScene::Draw()" << std::endl;
}
