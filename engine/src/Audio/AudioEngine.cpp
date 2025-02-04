
/// @file AudioEngine.cpp

#include "AudioEngine.hpp"

#include <FMOD/fmod_errors.h>
#include <iostream>

Sigma::AudioEngine::AudioEngine()
  : reverb(nullptr)
  , sounds()
  , loopsPlaying()
  , soundBanks()
  , eventDescriptions()
  , eventInstances()
{
}

void Sigma::AudioEngine::Init()
{
  
  ERRCHECK(FMOD::Studio::System::create(&studioSystem));
  ERRCHECK(studioSystem->getCoreSystem(&lowLevelSystem));
  ERRCHECK(lowLevelSystem->setSoftwareFormat(AUDIO_SAMPLE_RATE, FMOD_SPEAKERMODE_5POINT1, 0));
  ERRCHECK(lowLevelSystem->set3DSettings(1.0, DISTANCEFACTOR, 0.5f));

  //live-update while on debug
  ERRCHECK(studioSystem->initialize(MAX_AUDIO_CHANNELS, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));


  ERRCHECK(lowLevelSystem->getMasterChannelGroup(&mastergroup));
  InitializeReverb();

}

void Sigma::AudioEngine::Terminate()
{
  lowLevelSystem->close();
  studioSystem->release();
}

void Sigma::AudioEngine::Update() {
  ERRCHECK(studioSystem->update()); // also updates the low level system
}

void Sigma::AudioEngine::Load(AudioData audioData)
{
  PROFILER_START
  if (!audioData.IsLoaded())
  {
    std::cout << "[AudioEngine] Loading Sound from file " << audioData.GetFilePath() << '\n';
    FMOD::Sound* sound;
    ERRCHECK(lowLevelSystem->createSound(audioData.GetFilePath(), audioData.Is3D() ? FMOD_3D : FMOD_2D, 0, &sound));
    ERRCHECK(sound->setMode(audioData.Loop() ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF));
    ERRCHECK(sound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR));
    sounds.insert({ audioData.GetUniqueID(), sound });
    unsigned int msLength = 0;
    ERRCHECK(sounds[audioData.GetUniqueID()]->getLength(&msLength, FMOD_TIMEUNIT_MS));
    audioData.SetLengthMS(msLength);
    audioData.SetLoaded(true);
  }
  else
    std::cout << "[AudioEngine] Sound File was already loaded!\n";

  PROFILER_END("AudioEngine::Load")
}

void Sigma::AudioEngine::Play (const AudioData& audioData)
{
  if (!audioData.IsLoaded()) {
    std::cout << " [AudioEngine] Playing Sound: " << audioData.GetUniqueID() << "\n";
    FMOD::Channel* channel;
    // start play in 'paused' state
    ERRCHECK(lowLevelSystem->playSound(sounds[audioData.GetUniqueID()], nullptr, true /* start paused */, &channel));

    if (audioData.Is3D())
    {
      Set3DChannelPosition(audioData, channel);
    }

    //std::cout << "Playing sound at volume " << soundInfo.getVolume() << '\n';
    channel->setVolume(audioData.GetVolume());

    if (audioData.Loop()) // add to channel map of sounds currently playing, to stop later
      loopsPlaying.insert({ audioData.GetUniqueID(), channel });

    ERRCHECK(channel->setReverbProperties(0, audioData.GetReverbAmount()));

    // start audio playback
    ERRCHECK(channel->setPaused(false));

  }
  else
    std::cout << "[AudioEngine] Can't play, sound was not loaded yet from " << audioData.GetFilePath() << '\n';

}

void Sigma::AudioEngine::Stop(const AudioData& audioData)
{
  if (IsPlaying(audioData))
  {
    std::cout << "[AudioEngine] Stopping sound " << audioData.GetUniqueID() << '\n';
    ERRCHECK(loopsPlaying[audioData.GetUniqueID()]->stop());
    loopsPlaying.erase(audioData.GetUniqueID());
  }
  else
    std::cout << "[AudioEngine] Can't stop a looping sound that's not playing!\n";
}

void Sigma::AudioEngine::UpdateVolume(AudioData& audioData, float newVolume, unsigned int fadeSampleLength)
{
  if (IsPlaying(audioData))
  {
    FMOD::Channel* channel = loopsPlaying[audioData.GetUniqueID()];
    if (fadeSampleLength <= 64) // 64 samples is default volume fade out
      ERRCHECK(channel->setVolume(newVolume));
    else {
      bool fadeUp = newVolume > audioData.GetVolume();
      // get current audio clock time
      unsigned long long parentclock = 0;
      ERRCHECK(channel->getDSPClock(nullptr, &parentclock));

      float targetFadeVol = fadeUp ? 1.0f : newVolume;

      if (fadeUp) ERRCHECK(channel->setVolume(newVolume));

      ERRCHECK(channel->addFadePoint(parentclock, audioData.GetVolume()));
      ERRCHECK(channel->addFadePoint(parentclock + fadeSampleLength, targetFadeVol));
      //std::cout << "Current DSP Clock: " << parentclock << ", fade length in samples  = " << fadeSampleLength << "\n";
    }
    //std::cout << "Updating with new audio data volume \n";
    audioData.SetVolume(newVolume); // update the AudioData's volume
  }
  else
      std::cout << "[AudioEngine] Can't update sound loop volume! (It isn't playing or might not be loaded)\n";
}



void Sigma::AudioEngine::Update3DPosition(const AudioData& audioData)
{
  if (IsPlaying(audioData))
  {
      Set3DChannelPosition(audioData, loopsPlaying[audioData.GetUniqueID()]);
  }
  else
      std::cout << "[AudioEngine] Can't update sound position!\n";

}

bool Sigma::AudioEngine::IsPlaying(const AudioData& audioData)
{
return audioData.Loop() && loopsPlaying.contains(audioData.GetUniqueID());
}

void Sigma::AudioEngine::Set3DListenerPosition
(
  float posX, float posY, float posZ,
  float forwardX, float forwardY, float forwardZ,
  float upX, float upY, float upZ
)
{
  listenerPosition =  { posX,     posY,     posZ };
  forward =           { forwardX, forwardY, forwardZ };
  up =                { upX,      upY,      upZ };
  ERRCHECK(lowLevelSystem->set3DListenerAttributes(0, &listenerPosition, 0, &forward, &up));
}

unsigned int Sigma::AudioEngine::GetLengthMS(const AudioData& audioData)
{
  unsigned int length = 0;
  if (sounds.contains(audioData.GetUniqueID()))
      ERRCHECK(sounds[audioData.GetUniqueID()]->getLength(&length, FMOD_TIMEUNIT_MS));
  return length;
}

void Sigma::AudioEngine::LoadBank(const char* filepath)
{
  PROFILER_START
  std::cout << "[AudioEngine] Loading FMOD Studio Sound Bank " << filepath << '\n';
  FMOD::Studio::Bank* bank = nullptr;
  ERRCHECK(studioSystem->loadBankFile(filepath, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank));
  soundBanks.insert({ filepath, bank });
  PROFILER_END("AudioEngine::LoadBank")
}

void Sigma::AudioEngine::LoadEvent (const char* eventName, const std::vector<std::pair<const char*, float>>& paramsValues) // std::vector<std::map<const char*, float>> perInstanceParameterValues)
{
  PROFILER_START
  std::cout << "[AudioEngine] Loading FMOD Studio Event " << eventName << '\n';
  FMOD::Studio::EventDescription* eventDescription = nullptr;
  ERRCHECK(studioSystem->getEvent(eventName, &eventDescription));
  // Create an instance of the event
  FMOD::Studio::EventInstance* eventInstance = nullptr;
  ERRCHECK(eventDescription->createInstance(&eventInstance));
  for (const auto &[fst, snd] : paramsValues) {
    std::cout << "[AudioEngine] Setting Event Instance Parameter " << fst << " to value: " << snd << '\n';
    // Set the parameter values of the event instance
    ERRCHECK(eventInstance->setParameterByName(fst, snd));
  }
  eventInstances.insert({ eventName, eventInstance });
  eventDescriptions.insert({ eventName, eventDescription });
  PROFILER_END("AudioEngine::LoadEvent")
}

void Sigma::AudioEngine::SetEventParamValue(const char* eventName, const char* parameterName, float value)
{
  if (eventInstances.contains(eventName))
      ERRCHECK(eventInstances[eventName]->setParameterByName(parameterName, value));
  else
      std::cout << "[AudioEngine] Event " << eventName << " was not in event instance cache, can't set param \n";

}

void Sigma::AudioEngine::PlayEvent(const char* eventName, int instanceIndex) {
  // printEventInfo(eventDescriptions[eventName]);
  auto eventInstance = eventInstances[eventName];
  if (eventInstances.contains(eventName))
      ERRCHECK(eventInstances[eventName]->start());
  else
      std::cout << "[AudioEngine] Event " << eventName << " was not in event instance cache, cannot play \n";
}

void Sigma::AudioEngine::StopEvent(const char* eventName, int instanceIndex) {
  if (eventInstances.contains(eventName))
      ERRCHECK(eventInstances[eventName]->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT));
  else
      std::cout << "[AudioEngine] Event " << eventName << " was not in event instance cache, cannot stop \n";
}

void Sigma::AudioEngine::SetEventVolume(const char* eventName, float volume0to1)
{
  std::cout << "AudioEngine: Setting Event Volume\n";
  ERRCHECK(eventInstances[eventName]->setVolume(volume0to1));
  std::cout << "[AudioEngine] Setting Event Volume\n";
}

bool Sigma::AudioEngine::IsPlaying(const char* eventName, int instance /*= 0*/)
{
  FMOD_STUDIO_PLAYBACK_STATE playbackState;
  ERRCHECK(eventInstances[eventName]->getPlaybackState(&playbackState));
  return playbackState == FMOD_STUDIO_PLAYBACK_PLAYING;
}

void Sigma::AudioEngine::MuteAll()
{
  ERRCHECK(mastergroup->setMute(true));
  muted = true;
}

void Sigma::AudioEngine::UnmuteAll() {
  ERRCHECK(mastergroup->setMute(false));
  muted = false;
}
bool Sigma::AudioEngine::IsMute() const { return muted; }

// Private definitions
bool Sigma::AudioEngine::IsLoaded (const AudioData& audioData)
{
  //std::cout << "Checking sound " << soundInfo.getUniqueID() << " exists\n";
  return sounds.contains(audioData.GetUniqueID());
}

void Sigma::AudioEngine::Set3DChannelPosition (const AudioData& audioData, FMOD::Channel* channel) const
{
  FMOD_VECTOR position =
  {
    audioData.GetPosition().x * DISTANCEFACTOR,
    audioData.GetPosition().y * DISTANCEFACTOR,
    audioData.GetPosition().z * DISTANCEFACTOR
  };

  FMOD_VECTOR velocity = //TODO: Add doppler (velocity) support
  {
  0.0f,
  0.0f,
  0.0f
  };

  ERRCHECK(channel->set3DAttributes(&position, &velocity));
}

void Sigma::AudioEngine::InitializeReverb()
{
  ERRCHECK(lowLevelSystem->createReverb3D(&reverb));
  FMOD_REVERB_PROPERTIES prop2 = FMOD_PRESET_CONCERTHALL;
  ERRCHECK(reverb->setProperties(&prop2));
  ERRCHECK(reverb->set3DAttributes(&revPos, revMinDist, revMaxDist));
}

// Error checking/debugging function definitions

void ERRCHECK_fn(const FMOD_RESULT result, const char* file, const int line)
{
  if (result != FMOD_OK)
    std::cout << "FMOD ERROR: AudioEngine.cpp [Line " << line << "] " << result << "  - " << FMOD_ErrorString(result) << '\n';
}

void Sigma::AudioEngine::DebugEventInfo(const FMOD::Studio::EventDescription* eventDescription)
{
  int params;
  bool is3D, isOneshot;
  ERRCHECK(eventDescription->getParameterDescriptionCount(&params));
  ERRCHECK(eventDescription->is3D(&is3D));
  ERRCHECK(eventDescription->isOneshot(&isOneshot));

  std::cout << "FMOD EventDescription has " << params << " parameter descriptions, "
    << (is3D ? " is " : " isn't ") << " 3D, "
    << (isOneshot ? " is " : " isn't ") << " oneshot, "
    << (eventDescription->isValid() ? " is " : " isn't ") << " valid."
    << '\n';
}