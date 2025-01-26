/**
 * @file AnimationSystem.hpp
 * @author Dario 
 * @date 17/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once

namespace FNFE::ANIMATION {

//* @brief Struct to hold the frame data
struct Frame {
  std::string frameName;
  AEVec2 frameSize;
  AEVec2 framePosition;
  AEVec2 spriteSourceSize;
  AEVec2 spriteSourcePosition;
  AEVec2 pivot;
};

//* @brief Struct to hold the animation data
struct Animation {
  std::string animationName;
  float frameRate;
  std::vector<Frame> frames;
};

//* @brief Struct to hold the texture atlas data
struct TextureAtlas {
  std::string name;
  std::string filePath;
  AEVec2 size;
  AEGfxTexture *texture;
  std::vector<Animation> animations;
};

/**
 * @class AnimationSystem
 * @brief Shared functionality for animations 
 */
class AnimationSystem {
public:

  AnimationSystem() { m_AnimSysinstance = this; }
  ~AnimationSystem() { };

  /**
   * @brief Load a texture atlas from a json file
   * @param jsonFilePath Path to the json file
   * @return Pointer to the loaded texture atlas
   *
   * @todo Add support for multiple animations
   * @todo Add Support for pivot center
   */
  TextureAtlas* LoadTextureAtlas(const char* jsonFilePath);

  /**
   * @brief Get a loaded texture atlas by name
   * @param name Name of the texture atlas
   * @return Pointer to the texture atlas
   */
  TextureAtlas* GetTextureAtlas(const char *name);

  /**
   * @brief Build a texture transform matrix
   * @param pTexMtx Pointer to the texture matrix
   * @param sX X position of the frame
   * @param sY Y position of the frame
   * @param sW Width of the frame
   * @param sH Height of the frame
   * @param taW Width of the texture atlas
   * @param taH Height of the texture atlas
   *
   * @todo Add Support for trimmed sprites
   */
  void BuildTextureTransform(AEMtx33 *pTexMtx, float sX, float sY, float sW, float sH, float taW, float taH);

  /**
   * @brief Build a texture transform matrix
   * @param pTexMtx Pointer to the texture matrix
   * @param frame Pointer to the frame
   * @param atlas Pointer to the texture atlas
   */
  void BuildTextureTransform(AEMtx33 *pTexMtx, Frame* frame, TextureAtlas* atlas);

  
  static AnimationSystem* GetAnimationSysInstance() {
    if (m_AnimSysinstance == nullptr) {
      throw std::runtime_error("Animation System instance not created");
    }
    return m_AnimSysinstance;
  }
  
private:

  static AnimationSystem* m_AnimSysinstance; 
  
  std::unordered_map<std::string, TextureAtlas> m_loadedTextureAtlases;
  
};

}