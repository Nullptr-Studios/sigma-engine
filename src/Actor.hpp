/**
 * @file Actor.hpp
 * @author Xein
 * @date 14/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "Object.hpp"

namespace FNFE {

class Actor : public Object {
public:
  Actor(id_t id) : Object(id) { Init(); }

  void Init() override;
  void Start() override {}
  void Update(double delta) override {}
  void Draw() override {}
  void Destroy() override;

  AEGfxTriList * GetTris() const { return m_triangleList;}

  AEGfxTexture* GetTexture() const { return m_texture; }
  void SetTexturePath(const std::string& path) { m_texturePath = path; }

private:
  AEGfxTriList* m_triangleList;

  std::string m_texturePath = "res/toast.png";
  AEGfxTexture* m_texture = nullptr;
  void ReloadTexture();

};

typedef std::unordered_map<id_t, std::shared_ptr<Actor>> ActorMap;

}
