/**
 * @file UIProgressBar.hpp
 * @author dante
 * @date 1/25/2025
 *
 * @brief it's a progress bar that displaces progress based on a float
 * @note make sure to set the m_progress and transform.scale during creation
 */
#pragma once
#include "UIImage.hpp"
namespace Sigma {
class UIProgressBar : public UIImage{
public:
  explicit UIProgressBar(id_t id): UIImage(id) {}
  void Start() override;
  void Update(double delta) override;
  
  virtual void OnEnd() {};
  float m_progress{};
private:
  glm::vec2 m_scale{}; ///<@brief Scale of a 100% bar
};
}
