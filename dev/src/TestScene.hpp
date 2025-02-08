/**
 * @file TestScene.hpp
 * @author Dario
 * @date 11/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once

#include "Scene.hpp"

class TestScene : public Sigma::GameScene
{
public:
  
  TestScene(const char *name, unsigned ID, const char *jsonPath) : GameScene(name, ID, jsonPath) {}
  
  void Load() override;
  void Update(double delta) override;
  void Draw() override;

private:
  Sigma::Actor* m_actor;
};


