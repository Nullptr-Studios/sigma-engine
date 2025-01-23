/**
 * @file TestScene.hpp
 * @author Dario
 * @date 11/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once

#include "Scene.hpp"

class TestScene : public FNFE::Scene
{
public:

  TestScene(const char* name, unsigned index) : Scene(name, index) {}

  void Load() override;
  void Update(double delta) override;
  void Draw() override;
};


