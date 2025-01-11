/**
 * @file GameObject.hpp
 * @author Dario
 * @date 11/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "Object.hpp"

namespace FNFE {

class GameObject : public Object
{
  void Init() override;
  void Start() override;
  void Update(double delta) override;
  void Draw() override;
  void Destroy() override;
};

} // FNFE

