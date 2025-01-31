/**
 * @file Actor.hpp
 * @author Xein
 * @date 14/01/2025
 *
 * @brief The Actor class is a type of Object that can be rendered
 *
 * TODO: Dante add Collision things here
 * Maybe use it as a std::unique_ptr<CollisionComponent>, you have to create the CollisionComponent struct by yourself
 */

#pragma once
#include "AnimationSystem/AnimationComponent.hpp"
#include "Collision/Collision.hpp"
#include "Object.hpp"


namespace sigma::ANIMATION {
class AnimationComponent;
}

namespace sigma {
namespace Collision {
struct BoxCollider;
}

/**
 * @class Actor
 * @brief Object with renderable properties
 *
 * An Actor inherits from @c Object and adds renderable functions. All actors are also stored on the @c RenderablesMap
 * inside the Factory to allow for easier render function access
 */
class Actor : public Object {
public:
  Actor(id_t id) : Object(id) { Init(); }
  ~Actor() override = default;

  void Init() override;
  void Start() override {}
  void Update(double delta) override {}
  void Draw() override {}
  void Destroy() override;


  [[nodiscard]] AEGfxTexture* GetTexture() const { return m_texture; }  ///< @brief Gets the Alpha Engine texture pointer shit
  [[nodiscard]] Collision::BoxCollider* GetCollider() const { return m_collider; } ///< @brief Gets the collider component
  void SetCollider(Collision::BoxCollider *collider) { m_collider = collider; } ///< @brief Gets the collider component
  
  /**
   * @brief Sets the texture of the object
   * Changes the texture path variables and reloads the texture data by calling the factory
   * @param path Texture path
   */
  void SetTexture(const char* path);

  void SetTextureTransform(glm::mat3& newTexMtx);
  
  virtual glm::mat3& GetTextureTransform();

  bool IsInViewport();

protected:
  Collision::BoxCollider * m_collider = nullptr;
private:

  const char* m_texturePath = nullptr;
  AEGfxTexture* m_texture = nullptr;

  glm::mat3 m_tMtx; ///< @brief Texxure Matrix
};

}
