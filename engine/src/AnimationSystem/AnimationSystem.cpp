
#include "AnimationSystem.hpp"

#include <Core.hpp>
#include <Factory.hpp>


#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_transform_2d.hpp"

Sigma::Animation::AnimationSystem *Sigma::Animation::AnimationSystem::m_AnimSysinstance = nullptr;

// TODO: cleanup
// TODO: Support for trimmed sprites
// TODO: Support for pivot
Sigma::Animation::TextureAtlas *Sigma::Animation::AnimationSystem::LoadTextureAtlas(const char *jsonFilePath) {
  // Profiler time
  PROFILER_START;

  std::fstream file(jsonFilePath);
  if (!file.is_open()) {
    std::cout << "[AnimationSystem] failed to open JSON file " << jsonFilePath << '\n';
    return nullptr;
  }

  std::cout << "[AnimationSystem] Loading JSON file: " << jsonFilePath << '\n';
  json_t J = json_t::parse(file);

  // check if TextureAtlas is already loaded
  if (m_loadedTextureAtlases.contains(J["meta"]["image"])) {
    std::cout << "[AnimationSystem] Texture Atlas already loaded\n";
    file.close();
    return &m_loadedTextureAtlases[J["meta"]["image"]];
  }

  // default animation
  Animation a;
  if (J["meta"].contains("fps")) {
    a.frameRate = J["meta"]["fps"];
  } else {
    a.frameRate = 12;
  }
  a.name = "Default";

  std::vector<Animation> m_animations;
  m_animations.emplace_back(a);
  

  // first save all the frames
  // Then sort frames into animations

  for (auto frame: J["frames"]) {
    Frame f;

    f.name = frame["filename"];
    f.frameSize = {frame["frame"]["w"], frame["frame"]["h"]};
    f.framePosition = {frame["frame"]["x"], frame["frame"]["y"]};
    // f.rotated = frame["rotated"];
    f.spriteSourceSize = {frame["spriteSourceSize"]["w"], frame["spriteSourceSize"]["h"]};
    f.spriteSourcePosition = {frame["spriteSourceSize"]["x"], frame["spriteSourceSize"]["y"]};


    if (frame.contains("pivot")) {
      f.pivot = {frame["pivot"]["x"], frame["pivot"]["y"]};
    } else {
      // default to center
      f.pivot = {0.5f, 0.5f};
    }

    if (frame["trimmed"]) {
      f.sourceSize = {frame["sourceSize"]["w"], frame["sourceSize"]["h"]};
    } else {
      f.sourceSize = f.frameSize;
    }
    // m_frames.emplace_back(f);

    if (frame.contains("callback")) {
      f.AnimCallbackString = frame["callback"];
    }

    // Animation frame ownership
    auto index = f.name.find_last_of('_');
    std::string animName = f.name.substr(0, index);
    bool found = false;

    if (animName.contains(".png")) {
      // default animation
      m_animations[0].frames.emplace_back(f);
      found = true;
    } else {

      for (auto &m_animation: m_animations) {
        if (m_animation.name == animName) {
          m_animation.frames.emplace_back(f);
          found = true;
          break;
        }
      }

      if (!found) {
        Animation m_animation;

        if (J["meta"]["image"].contains("fps")) {
          m_animation.frameRate = J["meta"]["image"]["fps"];
        } else {
          m_animation.frameRate = 12;
        }

        m_animation.name = animName;
        m_animation.frames.emplace_back(f);
        m_animations.emplace_back(m_animation);
      }
    }
  }

  TextureAtlas ta;

  ta.size = {J["meta"]["size"]["w"], J["meta"]["size"]["h"]};
  ta.animations = m_animations;

  ta.textureStr = J["meta"]["image"];
  std::cout << "[AnimationSystem] Texture Atlas loaded\n";

  J.clear();
  file.close();

  PROFILER_END("AnimationSystem::LoadTextureAtlas");

  return &m_loadedTextureAtlases.emplace(ta.textureStr, ta).first->second;
}

Sigma::Animation::TextureAtlas *Sigma::Animation::AnimationSystem::GetTextureAtlas(const char *name) {
  // check if TextureAtlas is already loaded
  if (m_loadedTextureAtlases.contains(name)) {
    // std::cout << "[AnimationSystem] Texture Atlas already loaded\n";
    return &m_loadedTextureAtlases[name];
  }

  std::cout << "[AnimationSystem] Texture Atlas not found\n";
  return nullptr;
  
}
void Sigma::Animation::AnimationSystem::BuildTextureTransform(AEMtx33 *pTexMtx, float sX, float sY, float sW, float sH,
                                                              float taW, float taH) {
  float correctedY = taH - (sY + sH);
  float vTranslateY = correctedY / taH;

  float correctedX = (sX);
  float uTranslateX = correctedX / taW;

  float vHeight = (sH / taH);
  float uWidth = (sW / taW);

  // Reset to identity
  AEMtx33Identity(pTexMtx);

  // Scale
  AEMtx33ScaleApply(pTexMtx, pTexMtx, uWidth, vHeight);

  // Translate
  AEMtx33TransApply(pTexMtx, pTexMtx, uTranslateX, vTranslateY);
}

void Sigma::Animation::AnimationSystem::BuildTextureTransform(AEMtx33 *texMtx, const Frame *frame,
                                                              const TextureAtlas *atlas) {
  BuildTextureTransform(texMtx, frame->framePosition.x, frame->framePosition.y, frame->frameSize.x, frame->frameSize.y,
                        atlas->size.x, atlas->size.y);
}
void Sigma::Animation::AnimationSystem::UpdateSpriteOffset(Transform *transform, const Frame *frame,
                                                           const TextureAtlas *atlas)
{
  glm::vec3 pivotOffset = {((frame->pivot.x+.5f) * frame->spriteSourceSize.x) * transform->relativeScale.x,
                           ((frame->pivot.y-.5f) * frame->spriteSourceSize.y) * transform->relativeScale.y, 0};
  glm::vec3 trimOffset = {frame->spriteSourceSize.x * transform->relativeScale.x,
                          frame->spriteSourceSize.y * transform->relativeScale.y, 0};

  //DebugSpriteTransform(transform->position, transform->offset, frame->pivot, frame->spriteSourceSize, frame->spriteSourcePosition, transform->relativeScale,  -pivotOffset + trimOffset);
  transform->offset = -pivotOffset + trimOffset;
}

void Sigma::Animation::AnimationSystem::DebugSpriteTransform(const glm::vec2& position, const glm::vec2& offset, const  glm::vec2& pivot, 
                          const glm::vec2& spriteSourceSize, const glm::vec2& spriteSourcePosition,
                          const glm::vec2& relativeScale, const glm::vec2& finalPosition) 
{
  std::cout << "===== Debugging Sprite Transform =====\n";
  std::cout << "Transform Position: (" << position.x << ", " << position.y << ")\n";
  std::cout << "Offset: (" << offset.x << ", " << offset.y << ")\n";
  std::cout << "Pivot: (" << pivot.x << ", " << pivot.y << ")\n";
  std::cout << "Sprite Source Size: (" << spriteSourceSize.x << ", " << spriteSourceSize.y << ")\n";
  std::cout << "Sprite Source Position: (" << spriteSourcePosition.x << ", " << spriteSourcePosition.y << ")\n";
  std::cout << "Relative Scale: (" << relativeScale.x << ", " << relativeScale.y << ")\n";
  std::cout << "Final Position: (" << finalPosition.x << ", " << finalPosition.y << ")\n";
  std::cout << "======================================\n";
}