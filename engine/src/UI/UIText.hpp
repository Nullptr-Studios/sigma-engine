/**
 * @file UIText.hpp
 * @author dante
 * @date 1/25/2025
 *
 * @brief object that is text wow so cool
 */
#include "Objects/Object.hpp"

namespace sigma {
  class UIText final : public Object {
  public:
    explicit UIText(uint32_t id)
      : Object(id) {
    }

    void Init(const char * txt, unsigned color);

    void Update(double deltaTime) override;


    void Destroy() override;


  private:
    AEGfxFont *m_font{};
    std::string m_text;
    unsigned m_fontSize{};
    unsigned m_color{};
    float m_imgSize{};
  };
}
