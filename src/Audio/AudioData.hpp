// ?2023 JDSherbert. All rights reserved.

#pragma once

/// @file AudioData.h
/// 
/// Holds and provides data to be used by the AudioEngine.
///
/// @author JDSherbert

#pragma once
#include <aecore/AEVec3.h>
#include <string>


class AudioData
{
private:

    std::string uniqueID;
    const char* filePath;
    float volume;
    bool loaded;
    bool loop;
    bool is3D;
    unsigned int lengthMS;
    float reverbAmount;
    AEVec3 position;
    
public:

    AudioData();
    ~AudioData();

    std::string GetUniqueID() const { return uniqueID; };
    const char* GetFilePath() const { return filePath; }
    float GetVolume() const { return volume; }
    bool IsLoaded() const { return loaded; };
    bool Loop() const { return loop; };
    bool Is3D() const { return is3D; };
    float GetReverbAmount() const { return reverbAmount; }
    AEVec3 GetPosition() const { return position; }

    void SetLoaded(bool isLoaded) { loaded = isLoaded; }
    void SetLengthMS(unsigned int length) { lengthMS = length; }
    void SetVolume(float newVolume) { volume = newVolume; }

};