#include "Actor.hpp"

#include "AnimationSystem/AnimationComponent.hpp"
#include "Camera.hpp"
#include "Collision/Collider.hpp"
#include "Controller/CameraController.hpp"
#include "Factory.hpp"
#include "GameManager.hpp"


namespace Sigma {

void Actor::DebugWindow() {
  Object::DebugWindow();

  if (ImGui::CollapsingHeader("Render")) {
    char buffer[256];  // Allocate a buffer
    strncpy(buffer, m_texturePath, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    if (ImGui::InputText("Texture path", buffer, sizeof(buffer))) {
        m_texturePath = strdup(buffer);
    }

    if (ImGui::Button("Rebuild texture")) {
      SetTexture(m_texturePath);
    }

    if (ImGui::TreeNode("Texture transform")) {
      ImGui::Text("%.3f %.3f %.3f", m_tMtx[0][0], m_tMtx[1][0], m_tMtx[2][0]);
      ImGui::Text("%.3f %.3f %.3f", m_tMtx[0][1], m_tMtx[1][1], m_tMtx[2][1]);
      ImGui::Text("%.3f %.3f %.3f", m_tMtx[0][2], m_tMtx[1][2], m_tMtx[2][2]);
      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Color modulation")) {
      if (ImGui::ColorPicker4("Modulation color", m_color4, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar)) {
        m_color = AEGfxColor(m_color4[0], m_color4[1], m_color4[2], m_color4[3]);
      }
      ImGui::TreePop();
    }

  }

}

void Actor::SetTexture(const char *path) {
  m_texturePath = path;
  m_texture = GET_FACTORY->LoadTexture(m_texturePath);
}

void Actor::SetTextureTransform(const glm::mat3 &newTexMtx) { m_tMtx = newTexMtx; }

glm::mat3 *Actor::GetTextureTransform() { return &m_tMtx; }

bool Actor::IsInViewport() const {
  auto c = GET_CAMERA->GetCurrentCamera();
  int w = AEGetSysWindowWidth();
  int h = AEGetSysWindowHeight();

  auto pos = c->transform.position;
  return transform.position.x > pos.x - w / 2 && transform.position.x < pos.x + w / 2 &&
         transform.position.y > pos.y - h / 2 && transform.position.y < pos.y + h / 2;
}

} // namespace Sigma
