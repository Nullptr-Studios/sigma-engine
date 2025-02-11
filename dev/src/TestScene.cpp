/**
 * @file TestScene.cpp
 * @author Dario
 * @date 11/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#include "TestScene.hpp"
#include <iostream>

#include "Objects/CameraFollow.hpp"
#include "Controller/CameraController.hpp"
#include "Factory.hpp"
#include "Polygon.hpp"
#include "GlmAlphaTools.hpp"
#include "core.hpp"

void TestScene::Load() {
  GameScene::Load();
  std::cout << "TestScene::Load()" << std::endl;

  m_actor = GET_FACTORY->CreateObject<Sigma::Actor>("TestActor");
  m_actor->transform.offset = {-50.0f, 50.0f, 0.0f};
  m_camera = GET_FACTORY->CreateObject<Sigma::CameraFollow>("Main Camera");
  GET_CAMERA->SetCurrentCamera(m_camera);
  m_camera->m_target = m_actor;
  m_camera->m_safeZone = {-.25f,.25f,-.25f,.25f};
  m_camera->m_warningZone = {-.75f,.75f,-.75f,.75f};
}
void TestScene::Update(double delta) {
  GameScene::Update(delta);
  // std::cout << "TestScene::Update(" << delta << ")" << std::endl;
  m_actor->transform.localRotation += 2 * delta;
  auto mouseData = AEGetMouseData().position;
  auto mousePos = glm::FromAEX(mouseData);
  mousePos = m_camera->ScreenToWorld(mousePos);
  m_actor->transform.position = {mousePos.x,mousePos.y,0};
}
void TestScene::Draw() {
  GameScene::Draw();
  // std::cout << "TestScene::Draw()" << std::endl;
}
