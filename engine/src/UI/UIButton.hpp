/**
 * @file UIButton.hpp
 * @author dante
 * @date 1/25/2025
 *
 * @brief UI button for making things happen
 */
#pragma once
#include "UIImage.hpp"
namespace Sigma {
  class UIButton : public UIImage {
  public:
    explicit UIButton(id_t id): UIImage(id) {}

    void Init() override;

    void Update(double delta) override;

    virtual void OnClick() {}
    virtual void OnHoldClick() {}
    virtual void OnUnClick() {}
    virtual void OnHoverStart() {}
    virtual void OnHoverHold() {}
    virtual void OnHoverExit() {}

  private:
    bool m_pressed = false;
    bool m_hovered= false;
  };
}
