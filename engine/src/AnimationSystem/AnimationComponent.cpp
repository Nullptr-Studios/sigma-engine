/**
 * @file AnimationComponent.cpp
 * @author Dario
 * @date 18/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#include "AnimationComponent.hpp"

#include "AnimationSystem.hpp"
#include "Core.hpp"

void Sigma::ANIMATION::AnimationComponent::SetTextureAtlas(TextureAtlas* texAtlas)
{
  m_texAtlas = texAtlas;
  m_frameTime = 1.0f / m_texAtlas->animations[0].frameRate;

  m_timeSinceLastFrame = 0;
  
}

void Sigma::ANIMATION::AnimationComponent::SetCurrentAnim(std::string animName)
{
  if (m_currentAnimation!=nullptr)
    if (m_currentAnimation->name == animName)
      return;

  for (auto &anim: m_texAtlas->animations) {
    if (anim.name == animName) {
      m_currentAnimation = &anim;
      return;
    }
  }
  m_currentAnimation = nullptr;
}

void Sigma::ANIMATION::AnimationComponent::Update(double DeltaTime) {
  if (!m_isPlaying || m_currentAnimation == nullptr)
    return;

  if (m_timeSinceLastFrame > m_frameTime) {
    m_currentFrameIndex++;
    if (m_currentFrameIndex >= m_currentAnimation->frames.size()) {
      m_currentFrameIndex = 0;
    }
    m_currentFrame = &m_currentAnimation->frames[m_currentFrameIndex];
    m_timeSinceLastFrame = 0;
    UpdateTextureMatrix();
    UpdateCallbacks();
  } else {
    m_timeSinceLastFrame += DeltaTime;
  }
}

// TODO: Implement
void Sigma::ANIMATION::AnimationComponent::PlayAndStop() {}

// TODO: Implement
void Sigma::ANIMATION::AnimationComponent::GotoFrame(int frame) {}

void Sigma::ANIMATION::AnimationComponent::PlayAnim() {
  if (m_texAtlas == nullptr || m_currentAnimation == nullptr || m_isPlaying) return;

  // playing from the fist frame
  m_currentFrameIndex = 0;

  m_currentFrame = &m_currentAnimation->frames[m_currentFrameIndex];

  UpdateTextureMatrix();

  m_timeSinceLastFrame = 0;

  m_isPlaying = true;
}

void Sigma::ANIMATION::AnimationComponent::StopAnim() {
  if (m_currentAnimation == nullptr)
    return;

  m_isPlaying = false;
}
bool Sigma::ANIMATION::AnimationComponent::AddCallback(const std::string &callbackName,
                                                      const std::function<void()> &callback) {
  if (m_animCallbacks.contains(callbackName)) {
    std::cout << "[AnimationComponent] Callback name already exists\n";
    return false;
  }

  m_animCallbacks.emplace(callbackName, callback);
  return true;
}
void Sigma::ANIMATION::AnimationComponent::ClearCallbacks() {
  m_animCallbacks.clear();
}

void Sigma::ANIMATION::AnimationComponent::UpdateTextureMatrix() {
  if (m_currentFrame == nullptr || m_texAtlas == nullptr)
    return;
  GET_ANIMATION->BuildTextureTransform(m_texMtx, m_currentFrame, m_texAtlas);
}



void Sigma::ANIMATION::AnimationComponent::UpdateCallbacks()
{
  if (m_currentFrame->AnimCallbackString.empty())
    return;

  if (m_animCallbacks.contains(m_currentFrame->AnimCallbackString))
  {
    m_animCallbacks[m_currentFrame->AnimCallbackString]();
    std::cout << "[AnimationComponent] Callback: " << m_currentFrame->AnimCallbackString << " called\n";
    return;
  }

  std::cout << "[AnimationComponent] Callback: " << m_currentFrame->AnimCallbackString << " not found\n";

}
