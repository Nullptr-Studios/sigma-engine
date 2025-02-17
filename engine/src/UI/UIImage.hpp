/**
 
@file UIImage.hpp
@author Dante
@date 1/25/2025*
@brief UI superclass
*/
#pragma once
#include "Objects/Actor.hpp"

namespace Sigma {
  class UIImage : public Actor {
  public:
    UIImage(id_t id): Actor(id) {}
    void Start() override;
    void Update(double deltaTime) override;

    bool m_isScreenSpaceUI = true; ///<@brief all ui will be default set to screenspace poition
    Transform m_screenSpaceTransform{};
  };
} // Sigma
