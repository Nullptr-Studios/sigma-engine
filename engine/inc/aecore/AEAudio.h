// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEAudio.h
// Author			:	Thomas Komair
// Creation Date	:	2013/06/13
// Purpose			:	main header file for the AudioManager
// History			:
// - 2012/03/29		:	- initial implementation
// ---------------------------------------------------------------------------

#ifndef AE_AUDIO_MANAGER_H_
#define AE_AUDIO_MANAGER_H_

// STL includes
#include <list>
#include <map>
#include <string>

// Forward declaration of internal structures
struct Sound;
struct Voice;

// volume
float AEVoiceGetVolume(const Voice * pVoice);
void  AEVoiceSetVolume(Voice * pVoice, float volume);

// paused
void AEVoiceSetPause(Voice * pVoice, bool paused);
bool AEVoiceIsPaused(const Voice * pVoice);

// looping
void AEVoiceSetLoop(Voice * pVoice, bool isLoop);
bool AEVoiceIsLooping(const Voice * pVoice);

// group Id
void     AEVoiceSetGroupId(Voice * pVoice, unsigned group);
unsigned AEVoiceGetGroupId(const Voice * pVoice);

// Returns false when the voice finished playing.
// Note: this is meant for the gameplay code to know when a sound finished playing.
bool AEVoiceIsValid(const Voice * pVoice);

// Audio Manager
bool AEAudioInitialize();
void AEAudioUpdate();
void AEAudioShutdown();

// Debug Stats
unsigned AEAudioGetFreeVoiceCount();
unsigned AEAudioGetUsedVoiceCount();

// set group volumes
void AEAudioSetGroupVolume(unsigned group_id, float volume);
void AEAudioStopGroup(unsigned group_id);

// Sound Management
Sound * AEAudioCreateSound(const char * filename);
void    AEAudioFreeSound(Sound * pSound);
Voice * AEAudioPlay(Sound * pSound, bool paused = false, unsigned group_id = -1);
Voice * AEAudioLoop(Sound * pSound, bool paused = false, unsigned group_id = -1);

void AEAudioStopAll();

// Debug stats
unsigned AEAudioGetFreeVoiceCount();
unsigned AEAudioGetUsedVoiceCount();
unsigned AEAudioGetTotalVoiceCount();

// ----------------------------------------------------------------------------
#endif