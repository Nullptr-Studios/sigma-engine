/**
 * @file SceneButton.hpp
 * @author dante
 * @date 2/17/2025
 *
 * @brief Button That changes scenes
 */
#pragma once
#include "UI/UIButton.hpp"
namespace Sigma {
class Scene;
class SceneButton final : public Sigma::UIButton {
public:
  explicit SceneButton(const Sigma::id_t id, Sigma::Scene* scene, unsigned currentScene) : UIButton(id) {
    m_scene = scene;
    m_currentScene = currentScene;
  }
private:
  void OnClick() override;
  void OnHoverStart() override;
  void OnHoverExit() override;

  unsigned m_currentScene; ///<@brief scene to unload when changing scenes
  Sigma::Scene* m_scene; ///@brief scene to load when changing scenes

};
}
