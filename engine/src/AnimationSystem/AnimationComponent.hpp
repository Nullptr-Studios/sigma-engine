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
  explicit AnimationComponent(Sigma::Actor *owner) : m_owner(owner), m_texAtlas(nullptr), m_currentFrame(nullptr) {};
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

#pragma region Playback

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

#pragma endregion

#pragma region Callbacks
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
   * @brief Clear callbacks 
   */
  void ClearCallbacks();

#pragma endregion

  /**
   * @brief Get the texture matrix
   * @return Pointer to the texture matrix
   */
  [[nodiscard]] AEMtx33 GetTextureMatrix() const { return m_texMtx; }

  /**
   * @brief Get the texture atlas
   * @return Pointer to the texture atlas
   */
  [[nodiscard]] TextureAtlas *GetTextureAtlas() const { return m_texAtlas; }

  /**
   * @brief Get the current frame
   * @return Pointer to the current frame
   */
  [[nodiscard]] Frame *GetCurrentFrame() const { return m_currentFrame; }


  /**
   * @brief Set the loop of the animation
   * @param loop Loop the animation
   */
  void SetLoop(bool loop) { m_loop = loop; }

  /**
   * @brief Gets if the animation is playing
   * @return is playing
   */
  [[nodiscard]] bool IsPlaying() const { return m_isPlaying; }

  /**
   * @brief Get if the animation is looping
   * @return is looping
   */
  [[nodiscard]] bool IsLooping() const { return m_loop; }
  
#pragma region AnimationCallbacks
  /**
   * @brief Set the callback for the end of the animation
   * @param callback Callback function
   */
  void SetOnAnimationEnd(std::function<void(const std::string&)> callback) { m_onAnimationEnd = std::move(callback); }

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

#pragma endregion


private:

  // Animation Event callbacks
  // tell me if you want more -d
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
