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

void sigma::ANIMATION::AnimationComponent::SetTextureAtlas(TextureAtlas* texAtlas)
{
  m_texAtlas = texAtlas;
  m_frameTime = 1.0f / m_texAtlas->animations[0].frameRate;

  m_timeSinceLastFrame = 0;
  
}

void sigma::ANIMATION::AnimationComponent::SetCurrentAnim(std::string animName)
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

void sigma::ANIMATION::AnimationComponent::Update(double DeltaTime)
{
  if (!m_isPlaying || m_currentAnimation == nullptr) return;

  if (m_timeSinceLastFrame > m_frameTime)
  {
    m_currentFrameIndex++;
    if (m_currentFrameIndex >= m_currentAnimation->frames.size())
    {
      m_currentFrameIndex = 0;
    }
    m_currentFrame = &m_currentAnimation->frames[m_currentFrameIndex];
    m_timeSinceLastFrame = 0;
    UpdateTextureMatrix();
  }
  else
  {
    m_timeSinceLastFrame += DeltaTime;
  }
}

void sigma::ANIMATION::AnimationComponent::PlayAnim() {
  if (m_texAtlas == nullptr || m_currentAnimation == nullptr || m_isPlaying) return;

  // playing from the fist frame
  m_currentFrameIndex = 0;

  m_currentFrame = &m_currentAnimation->frames[m_currentFrameIndex];

  UpdateTextureMatrix();

  m_timeSinceLastFrame = 0;

  m_isPlaying = true;
}

void sigma::ANIMATION::AnimationComponent::StopAnim()
{
  if (m_currentAnimation == nullptr) return;

  m_isPlaying = false;
}

void sigma::ANIMATION::AnimationComponent::UpdateTextureMatrix()
{
  if (m_currentFrame == nullptr || m_texAtlas == nullptr)
    return;
  FNFE_ANIMATION->BuildTextureTransform(m_texMtx, m_currentFrame, m_texAtlas);
}
