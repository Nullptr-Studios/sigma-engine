/**
 * @file TestScene.cpp
 * @author Dario
 * @date 11/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#include "TestScene.hpp"
#include <iostream>

#include "Factory.hpp"
#include "Polygon.hpp"
#include "core.hpp"

void TestScene::Load() {
  GameScene::Load();
  std::cout << "TestScene::Load()" << std::endl;

  m_actor = GET_FACTORY->CreateObject<Sigma::Actor>("TestActor");
  m_actor->transform.offset = {-50.0f, 50.0f, 0.0f};

}
void TestScene::Update(double delta) {
  GameScene::Update(delta);
  auto mouse = AEGetMouseData();
  glm::vec2 mousePos = {mouse.position.x, mouse.position.y};
  //std::string mousePosStr = "Mouse Pos: " + std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y);
  std::string inside = GetSceneBoundsPoly()->isPointInside(mousePos) ? "Inside" : "Outside";
  std::cout << inside << std::endl;
  // std::cout << "TestScene::Update(" << delta << ")" << std::endl;
  m_actor->transform.localRotation += 2 * delta;
}
void TestScene::Draw() {
  GameScene::Draw();
  // std::cout << "TestScene::Draw()" << std::endl;
}
