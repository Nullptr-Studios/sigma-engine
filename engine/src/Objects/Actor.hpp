/**
 * @file Actor.hpp
 * @author Xein
 * @date 14/01/2025
 *
 * @brief The Actor class is a type of Object that can be rendered
 */

#pragma once
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
  explicit Actor(id_t id) : Object(id) {}
  ~Actor() override = default;

  void Init() override {
    Object::Init();
    std::cout << "Init called \n";
  };
  void Start() override { Object::Start(); };
  void Update(double delta) override { Object::Update(delta); };
  void Draw() override { Object::Draw(); };
  void Destroy() override { Object::Destroy(); };


  [[nodiscard]] AEGfxTexture* GetTexture() const { return m_texture; }  ///< @brief Gets the Alpha Engine texture pointer shit
  
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
  glm::mat3 m_tMtx = glm::mat3(1.0f); ///< @brief Texture Matrix

private:
  const char* m_texturePath = nullptr;
  AEGfxTexture* m_texture = nullptr;
};

}
