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
  ~Actor() override = default;

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

  // TODO: Modulation color does not actually work idunno why -d
  void SetModulationColor(unsigned newColor) { m_color = newColor; }
  [[nodiscard]] unsigned GetModulationColor() const { return m_color; }

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
  const char *m_texturePath = nullptr;
  AEGfxTexture *m_texture = nullptr;

  unsigned m_color = AE_COLORS_WHITE;
};

} // namespace Sigma
