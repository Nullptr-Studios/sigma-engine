/**
 * @file UIProgressBar.hpp
 * @author dante
 * @date 1/25/2025
 *
 * @brief its a progress bar that displaces progress based on a float
 */
#pragma once
#include "UIImage.hpp"

namespace Sigma {


class UIProgressBar final : public UIImage{
public:
  explicit UIProgressBar(id_t id): UIImage(id) {}
  void Start() override;
  void Update(double delta) override;
  
  virtual void OnEnd() {};
  float m_progress{};

private:
  glm::vec2 m_scale{};
};
}
