/**
 * @file Actor.hpp
 * @author Xein
 * @date 14/01/2025
 *
 * @brief The Actor class is a type of Object that can be rendered
 */

#pragma once
#include "Object.hpp"
#include "AnimationSystem/AnimationComponent.hpp"
#include "aecore/AEGraphics.h"
#include "core.hpp"

namespace Sigma {

/**
 * @class Actor
 * @brief Object with renderable properties
 *
 * An Actor inherits from @c Object and adds renderable functions. All actors are also stored on the @c RenderablesMap
 * inside the Factory to allow for easier render function access
 */
class Actor : public Object {
public:
  explicit Actor(id_t id) : Object(id) {}
  ~Actor() override {
    if (m_mesh) AEGfxTriFree(m_mesh);
  };

  void Init() override { Object::Init(); };
  void Start() override { Object::Start(); };
  void Update(double delta) override { Object::Update(delta); };
  void DebugWindow() override;
  void Draw() override { Object::Draw(); };
  void Destroy() override { Object::Destroy(); };

#pragma region RendererStuff

  /**
   * @brief Gets the texture of the object
   * @returns Texture pointer
   */
  [[nodiscard]] AEGfxTexture *GetTexture() const {
    return m_texture;
  } 

  /**
   * @brief Sets the texture of the object
   * Changes the texture path variables and reloads the texture data by calling the factory
   * @param path Texture path
   */
  void SetTexture(const char *path);

  /**
   * @brief Sets the texture transform matrix
   * @param newTexMtx New texture matrix
   */
  void SetTextureTransform(const glm::mat3 &newTexMtx);

  /**
   * @brief Gets the texture transform matrix
   * @returns Texture matrix
   */
  [[nodiscard]]
  virtual glm::mat3 *GetTextureTransform();

  glm::vec4 GetTint() const { return m_tint; }
  unsigned GetTintAEX() const { return AEGfxColor(m_tint.r * 255, m_tint.g * 255, m_tint.b *255, m_tint.a * 255); }
  void SetTint(glm::vec4 color) { m_tint = color; }

  AEGfxTriList* GetMesh() { return m_mesh; }

  /**
   * @brief checks if an actor is in view bounds
   * @note it does not check the bounding box, only a point
   * @returns if in viewport
   */
  [[nodiscard]] bool IsInViewport() const;

#pragma endregion 

  /**
   * @brief animation component pointer
   * @note Is not initialized by default.
   */
  std::unique_ptr<Animation::AnimationComponent> m_animComp;

protected:
  glm::mat3 m_tMtx = glm::mat3(1.0f); ///< @brief Texture Matrix

private:
  void CreateMesh() {
    AEGfxTriStart();
    AEGfxTriAdd(-0.5f, -0.5f, AE_COLORS_WHITE, 0.0f, 0.0f, -0.5f, 0.5f, AE_COLORS_WHITE, 0.0f, 1.0f, 0.5f, -0.5f,
                AE_COLORS_WHITE, 1.0f, 0.0f);
    AEGfxTriAdd(-0.5f, 0.5f, AE_COLORS_WHITE, 0.0f, 1.0f, 0.5f, 0.5f, AE_COLORS_WHITE, 1.0f, 1.0f, 0.5f, -0.5f,
                AE_COLORS_WHITE, 1.0f, 0.0f);

    m_mesh = AEGfxTriEnd();
  }

  const char *m_texturePath = nullptr;
  AEGfxTexture *m_texture = nullptr;
  AEGfxTriList* m_mesh = nullptr;
  glm::vec4 m_tint = glm::vec4(1.0f);

};

} // namespace Sigma
