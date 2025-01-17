/**
 * @file Actor.hpp
 * @author Xein
 * @date 14/01/2025
 *
 * @brief The Actor class is a type of Object that can be rendered
 *
 * TODO: Dante add collision things here
 * Maybe use it as a std::unique_ptr<CollisionComponent>, you have to create the CollisionComponent struct by yourself
 */

#pragma once
#include "Object.hpp"


struct RectCollider;
namespace FNFE {

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

  [[nodiscard]] AEGfxTriList* GetTris() const { return m_triangleList;} ///< @brief Gets the Alpha Engine triangle list shit
  [[nodiscard]] AEGfxTexture* GetTexture() const { return m_texture; }  ///< @brief Gets the Alpha Engine texture pointer shit
  RectCollider* GetCollider() const { return m_collider; } ///< @brief Gets the collider component
  /**
   * @brief Sets the texture of the object
   * Changes the texture path variables and reloads the texture data by calling the factory
   * @param path Texture path
   */
  void SetTexture(const char* path);

protected:
  RectCollider * m_collider = nullptr;
private:
  AEGfxTriList* m_triangleList;

  const char* m_texturePath = nullptr;
  AEGfxTexture* m_texture = nullptr;
};

typedef std::unordered_map<id_t, std::shared_ptr<Actor>> ActorMap;

}
