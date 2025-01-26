/**
 * @file AnimationComponent.hpp
 * @author Dario
 * @date 18/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once

namespace FNFE::ANIMATION {
struct Animation;
}

namespace FNFE::ANIMATION {

struct Frame;
struct TextureAtlas;

/**
 * @class AnimationComponent
 */
class AnimationComponent 
{
public:

  AnimationComponent() = default;
  ~AnimationComponent() = default;

  /**
   * @brief Set the texture atlas for the animation component
   * @param texAtlas Pointer to the texture atlas
   */
  void SetTextureAtlas(TextureAtlas* texAtlas);

  void SetCurrentAnim(std::string animName);
  
  void Update(double DeltaTime);

  
  void PlayAnim();

  void StopAnim();

  /**
   * @brief Get the texture matrix
   * @return Pointer to the texture matrix
   */
  AEMtx33* GetTextureMatrix() { return &m_texMtx; }

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
  
  AEMtx33 m_texMtx;
  TextureAtlas* m_texAtlas;
  int m_currentFrameIndex = 0;
  double m_frameTime = 0.0;
  Frame* m_currentFrame;

  Animation* m_currentAnimation;

  double m_timeSinceLastFrame = 0.0;

  bool m_isPlaying = false;
  
  
};

}
