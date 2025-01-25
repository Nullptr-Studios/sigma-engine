/**
 * @file UIText.hpp
 * @author dante
 * @date 1/25/2025
 *
 * @brief object that is text wow so cool
 */
#include "Objects/Object.hpp"

namespace FNFE {
  class UIText final : public Object {
  public:
    explicit UIText(uint32_t id)
      : Object(id) {
    }

    void Init() override;

    void Update(double deltaTime) override;


    void Destroy() override;

  private:
    AEGfxFont *m_font{};
    float m_fontSize{};
    float m_imgSize{};
  };
}
