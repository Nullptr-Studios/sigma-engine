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
class AnimationComponent {
public:
  AnimationComponent() = default;
  ~AnimationComponent() { ClearCallbacks(); }

  /**
   * @brief Set the texture atlas for the animation component
   * @param texAtlas Pointer to the texture atlas
   */
  void SetTextureAtlas(TextureAtlas *texAtlas);

  /**
   * @brief Set the current animation
   * @param animName Name of the animation
   */
  void SetCurrentAnim(const std::string &animName);

  /**
   * @brief Update the animation component
   * @param DeltaTime Time since last frame
   */
  void Update(double DeltaTime);

  /**
   * @brief Play and stop the animation
   */
  void PlayAndStop();

  /**
   * @brief Go to a specific frame
   * @param frame Frame to go to
   */
  void GotoFrame(int frame);

  /**
   * @brief Play the animation
   */
  void PlayAnim();

  /**
   * @brief Stop the animation
   */
  void StopAnim();

  /**
   * @brief Add a callback to the animation
   * @param callbackName Name of the callback
   * @param callback Callback function
   * @return True if the callback was added successfully
   */
  bool AddCallback(const std::string &callbackName, const std::function<void()> &callback);

  /**
   * @brief Remove a callback from the animation
   * @param callbackName Name of the callback
   */
  void ClearCallbacks();

  /**
   * @brief Get the texture matrix
   * @return Pointer to the texture matrix
   */
  glm::mat3 &GetTextureMatrix() { return m_texMtx; }

  /**
   * @brief Get the texture atlas
   * @return Pointer to the texture atlas
   */
  TextureAtlas *GetTextureAtlas() { return m_texAtlas; }

  /**
   * @brief Get the current frame
   * @return Pointer to the current frame
   */
  Frame *GetCurrentFrame() { return m_currentFrame; }

private:
  /**
   * @brief Update the texture matrix
   */
  void UpdateTextureMatrix();

  /**
   * @brief Update the callbacks
   */
  void UpdateCallbacks();

  /**
   * @brief Map of animation callbacks
   */
  AnimationCallbackMap m_animCallbacks;

  glm::mat3 m_texMtx{}; ///< @brief Texture matrix
  TextureAtlas *m_texAtlas{}; ///< @brief Texture atlas

  Frame *m_currentFrame{};
  Animation *m_currentAnimation{};

  int m_currentFrameIndex = 0;
  double m_frameTime = 0.0;
  double m_timeSinceLastFrame = 0.0;

  bool m_isPlaying = false;
  bool m_loop = true;
};

} // namespace FNFE::ANIMATION
