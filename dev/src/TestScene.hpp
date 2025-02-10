/**
 * @file TestScene.hpp
 * @author Dario
 * @date 11/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once

#include "Scene.hpp"

namespace Sigma {
class Actor;
class CameraFollow;
class Camera;
}
class TestScene : public Sigma::Scene
{
public:

  TestScene(const char* name, unsigned index) : Scene(name, index) {}

  void Load() override;
  void Update(double delta) override;
  void Draw() override;

private:
  Sigma::Actor* m_actor;
  Sigma::CameraFollow* m_camera;
};


