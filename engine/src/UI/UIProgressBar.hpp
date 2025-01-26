/**
 * @file UIProgressBar.hpp
 * @author dante
 * @date 1/25/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#include "UIImage.hpp"

namespace FNFE {


class UIProgressBar final : public UIImage{
public:
  explicit UIProgressBar(id_t id): UIImage(id) {}
  void Init() override;
  void Update(double delta) override;
  virtual void OnEnd() {};
  AEVec2 m_scale = {750, 25};
  float m_progress = 1;

private:
};
}
