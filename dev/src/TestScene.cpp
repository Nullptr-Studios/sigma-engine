/**
 * @file TestScene.cpp
 * @author Dario
 * @date 11/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#include "TestScene.hpp"
#include <iostream>

#include "Controller/CameraController.hpp"
#include "Objects/Camera.hpp"
#include "Factory.hpp"
#include "core.hpp"

void TestScene::Load() {
  Scene::Load();
  std::cout << "TestScene::Load()" << std::endl;

}
void TestScene::Init() {
  m_camea = GET_FACTORY->CreateObject<Sigma::Camera>("Main Camea");
  m_background = GET_FACTORY->CreateObject<Sigma::Actor>("Background");
  m_background->transform.scale = {1000,1000};
  m_background->SetTexture("res/LotsOfBoxes.png");
  GET_CAMERA->SetCurrentCamera(m_camea);
  Scene::Init();
}
void TestScene::Update(double delta) {
  Scene::Update(delta);
  // std::cout << "TestScene::Update(" << delta << ")" << std::endl;
}
void TestScene::Draw() {
  Scene::Draw();
  // std::cout << "TestScene::Draw()" << std::endl;
}
