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
// #include "Objects/Character.hpp"
#include "pch.hpp" 


namespace Sigma {
class Actor;

namespace Animation {

struct Animation;
struct Frame;
struct TextureAtlas;


/**
 * @class AnimationComponent
 */
class AnimationComponent {
public:
  AnimationComponent(Sigma::Actor *owner) : m_owner(owner) {};
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
   *
   * @note You will need to add in the json frame a member called "callback" with the name of the callback in order to be caller
   */
  bool AddCallback(const std::string &callbackName, const std::function<void(std::string, unsigned short, bool)> &callback);

  /**
   * @brief Remove a callback from the animation
   * @param callbackName Name of the callback
   */
  void RemoveCallback(const std::string &callbackName);

  /**
   * @brief Remove a callback from the animation
   * @param callbackName Name of the callback
   */
  void ClearCallbacks();

  /**
   * @brief Get the texture matrix
   * @return Pointer to the texture matrix
   */
  AEMtx33 GetTextureMatrix() { return m_texMtx; }

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


  void SetLoop(bool loop) { m_loop = loop; }

  bool IsPlaying() const { return m_isPlaying; }

  bool IsLooping() const { return m_loop; }
  
  // Playback events
  /**
   * @brief Set the callback for the end of the animation
   * @param callback Callback function
   */
  void SetOnAnimationEnd(const std::function<void(std::string)> &callback) { m_onAnimationEnd = callback; }

  /**
   * @brief Clear the callback for the end of the animation
   */
  void ClearOnAnimationEnd() { m_onAnimationEnd = nullptr; }

  /**
   * @brief Set the callback for the change of frame
   * @param callback Callback function
   */
  void SetOnAnimationChangeFrame(const std::function<void(std::string,short)> &callback) { m_onAnimationChangeFrame = callback; }

  /**
   * @brief Clear the callback for the change of frame
   */
  void ClearOnAnimationChangeFrame() { m_onAnimationChangeFrame = nullptr; }


private:

  std::function<void(std::string)> m_onAnimationEnd;
  std::function<void(std::string,unsigned short)> m_onAnimationChangeFrame;
  
  /**
   * @brief Update the texture matrix
   */
  void UpdateTextureMatrix();

  /**
   * @brief Update the callbacks
   */
  void UpdateCallbacks();

  Sigma::Actor *m_owner;

  /**
   * @brief Map of animation callbacks
   */
  AnimationCallbackMap m_animCallbacks;

  AEMtx33 m_texMtx;
  TextureAtlas *m_texAtlas;
  int m_currentFrameIndex = 0;
  double m_frameTime = 0.0;
  Frame *m_currentFrame;

  Animation *m_currentAnimation = nullptr;

  double m_timeSinceLastFrame = 0.0;

  bool m_isPlaying = false;
  bool m_loop = true;
};

} // namespace Animation
} // namespace Sigma
