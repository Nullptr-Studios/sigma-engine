/**
 * @file AnimationComponent.hpp
 * @author Dario
 * @date 18/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "AnimationSystem.hpp"
#include "GlmAlphaTools.hpp"

namespace FNFE::ANIMATION {

struct Animation;
struct Frame;
struct TextureAtlas;


/**
 * @class AnimationComponent
 */
class AnimationComponent 
{
public:

  AnimationComponent() = default;
  ~AnimationComponent() { ClearCallbacks(); }

  /**
   * @brief Set the texture atlas for the animation component
   * @param texAtlas Pointer to the texture atlas
   */
  void SetTextureAtlas(TextureAtlas* texAtlas);

  void SetCurrentAnim(std::string animName);
  
  void Update(double DeltaTime);

  void PlayAndStop();

  void GotoFrame(int frame);
  
  void PlayAnim();

  void StopAnim();

  bool AddCallback(const std::string& callbackName, const std::function<void()>& callback);

  void ClearCallbacks();

  /**
   * @brief Get the texture matrix
   * @return Pointer to the texture matrix
   */
  glm::mat3& GetTextureMatrix() { return m_texMtx; }

  /**
   * @brief Get the texture atlas
   * @return Pointer to the texture atlas
   */
  TextureAtlas* GetTextureAtlas() { return m_texAtlas; }

  /**
   * @brief Get the current frame
   * @return Pointer to the current frame
   */
  Frame* GetCurrentFrame() { return m_currentFrame; }

private:

  /**
   * @brief Update the texture matrix
   */
  void UpdateTextureMatrix();

  void UpdateCallbacks();

  AnimationCallbackMap m_animCallbacks;
  
  glm::mat3 m_texMtx = glm::mat3(1.0f);
  TextureAtlas* m_texAtlas;
  int m_currentFrameIndex = 0;
  double m_frameTime = 0.0;
  Frame* m_currentFrame;

  Animation* m_currentAnimation;

  double m_timeSinceLastFrame = 0.0;

  bool m_isPlaying = false;
  
  
};

}
