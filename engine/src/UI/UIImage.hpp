//
// Created by Pedro on 25/01/2025.
//

/**
 
@file UIImage.hpp
@author Dante
@date 1/25/2025*
@brief UI superclass
*/
#pragma once
#include "Objects/Actor.hpp"

namespace FNFE {
  class UIImage : public Actor {
  public:
    void Init() override;
    void Update(double delta) override;;
    unsigned m_uiLayer{};

  };
} // FNFE
