#include "SceneButton.hpp"
#include "GameManager.hpp"
namespace Sigma {
void SceneButton::OnClick() {
  UIButton::OnClick();
  if (m_scene == nullptr) {
    std::cout << "Quiting Game...\n";
    GET_MANAGER->Uninitialize();
    AESysExit();
    return;
  }
  std::cout << "Changing Scene...\n";
  GET_MANAGER->LoadScene(m_scene);
  GET_MANAGER->UnloadScene(m_currentScene);
}
void SceneButton::OnHoverStart() {
  UIButton::OnHoverStart();
  transform.scale *= 1.05;
  m_screenSpaceTransform.scale *= 1.05f;
}
void SceneButton::OnHoverExit() {
  UIButton::OnHoverExit();
  transform.scale /= 1.05;
  m_screenSpaceTransform.scale /= 1.05f;
}
}
