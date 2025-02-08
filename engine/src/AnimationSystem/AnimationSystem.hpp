/**
 * @file AnimationSystem.hpp
 * @author Dario
 * @date 17/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include <aecore/AEEngine.h>
#include "glm/fwd.hpp"
#include "glm/vec2.hpp"

namespace Sigma {
struct Transform;
}
namespace Sigma::Animation {

/**
 * @brief Type definition for the animation callback map
 *
 * @note string is the name of the callback
 * @note unsigned short is the frame index
 * @note bool is the loop status
 */
typedef std::unordered_map<std::string, std::function<void(std::string, unsigned short, bool)>>
    AnimationCallbackMap; ///< @typedef AnimationCallbackMap @brief Type definition for the animation callback map

/// @brief Struct to hold the frame data
struct Frame {
  std::string name;
  glm::vec2 frameSize;
  glm::vec2 framePosition;

  // Only available when trimmed
  glm::vec2 spriteSourcePosition;
  glm::vec2 spriteSourceSize;
  glm::vec2 sourceSize;
  
  glm::vec2 pivot;
  
  std::string AnimCallbackString;
};

/// @brief Struct to hold the animation data
struct Animation {
  std::string name;
  float frameRate;
  std::vector<Frame> frames;
};

/// @brief Struct to hold the texture atlas data
struct TextureAtlas {
  std::string name;
  glm::vec2 size;
  std::string textureStr;
  std::vector<Animation> animations;
};

/**
 * @class AnimationSystem
 * @brief Shared functionality for animations
 */
class AnimationSystem {
public:
  AnimationSystem() { m_AnimSysinstance = this; }
  ~AnimationSystem() = default;

  /**
   * @brief Load a texture atlas from a json file
   * @param jsonFilePath Path to the json file
   * @return Pointer to the loaded texture atlas
   *
   * @todo Add support for multiple animations
   * @todo Add Support for pivot center
   */
  TextureAtlas *LoadTextureAtlas(const char *jsonFilePath);

  /**
   * @brief Get a loaded texture atlas by name
   * @param name Name of the texture atlas
   * @return Pointer to the texture atlas
   */
  TextureAtlas *GetTextureAtlas(const char *name);

  /**
   * @brief Build a texture transform matrix
   * @param texMtx Texture matrix
   * @param framePosition Position of the animation frame in the atlas
   * @param frameSize Size of the animation frame in the atlas
   * @param atlasSize Size of the texture atlas
   *
   * @todo Add Support for trimmed sprites
   */
  void BuildTextureTransform(AEMtx33 *pTexMtx, float sX, float sY, float sW, float sH, float taW, float taH);

  /**
   * @brief Build a texture transform matrix
   * @param texMtx Texture matrix
   * @param frame Pointer to the frame
   * @param atlas Pointer to the texture atlas
   */
  void BuildTextureTransform(AEMtx33 *texMtx, const Frame *frame, const TextureAtlas *atlas);
  
  /**
   * @brief Update the sprite offset
   * @param transform Pointer to the transform
   * @param frame Pointer to the frame
   * @param atlas Pointer to the texture atlas
   */
  void UpdateSpriteOffset(Transform *transform, const Frame *frame, const TextureAtlas *atlas);


  /**
   * @brief Get the instance of the Animation System
   * @return Pointer to the Animation System instance
   */
  static AnimationSystem *GetInstance() {
    if (m_AnimSysinstance == nullptr) {
      throw std::runtime_error("Animation System instance not created");
    }
    return m_AnimSysinstance;
  }

private:
  static AnimationSystem *m_AnimSysinstance;

  /**
   * @brief Debug function to draw the sprite transform
   * @param position Position of the sprite
   * @param offset Offset of the sprite
   * @param pivot Pivot of the sprite
   * @param spriteSourceSize Size of the sprite source
   * @param spriteSourcePosition Position of the sprite source
   * @param relativeScale Relative scale of the sprite
   * @param finalPosition Final position of the sprite
   */
  void DebugSpriteTransform(const glm::vec2& position, const glm::vec2& offset, const  glm::vec2& pivot, 
                          const glm::vec2& spriteSourceSize, const glm::vec2& spriteSourcePosition,
                          const glm::vec2& relativeScale, const glm::vec2& finalPosition); 

  std::unordered_map<std::string, TextureAtlas> m_loadedTextureAtlases;
};

} // namespace Sigma::Animation
