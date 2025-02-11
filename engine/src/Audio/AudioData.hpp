
#pragma once

/// @file AudioData.h
///
/// Holds and provides data to be used by the AudioEngine.

#pragma once
#include <glm/vec3.hpp>
#include <string>

class AudioData {

  std::string uniqueID;
  const char *filePath;
  float volume;
  bool loaded;
  bool loop;
  bool is3D;
  unsigned int lengthMS;
  float reverbAmount;
  glm::vec3 position;

public:
  explicit AudioData(const char *filePath, bool loop = false, bool is3D = false, float reverbAmount = 0.0f,
            glm::vec3 position = {0.0f, 0.0f, 0.0f}) :
      filePath(filePath), loop(loop), is3D(is3D), reverbAmount(reverbAmount), position(position) {
    volume = 1.0f;
    loaded = false;
    lengthMS = 0;
    uniqueID = filePath;
  }
  ~AudioData() = default;

  [[nodiscard]] std::string GetUniqueID() const { return uniqueID; };
  [[nodiscard]] const char *GetFilePath() const { return filePath; }
  [[nodiscard]] float GetVolume() const { return volume; }
  [[nodiscard]] bool IsLoaded() const { return loaded; };
  [[nodiscard]] bool Loop() const { return loop; };
  [[nodiscard]] bool Is3D() const { return is3D; };
  [[nodiscard]] float GetReverbAmount() const { return reverbAmount; }
  [[nodiscard]] glm::vec3 GetPosition() const { return position; }

  void SetLoaded(bool isLoaded) { loaded = isLoaded; }
  void SetLengthMS(unsigned int length) { lengthMS = length; }
  void SetVolume(float newVolume) { volume = newVolume; }
};
