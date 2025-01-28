/**
 * @file TestScene.cpp
 * @author Dario
 * @date 11/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#include "TestScene.hpp"
#include <iostream>

void TestScene::Load() {
  Scene::Load();
  std::cout << "TestScene::Load()" << std::endl;
  
  
}
void TestScene::Update(double delta) {
  Scene::Update(delta);
  std::cout << "TestScene::Update(" << delta << ")" << std::endl;
}
void TestScene::Draw() {
  Scene::Draw();
  std::cout << "TestScene::Draw()" << std::endl;
}
