#ifndef __MODULE_AUDIO_H__
#define __MODULE_AUDIO_H__

#include "Globals.h"
#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

class ModuleAudio : public Module
{
public:
	ModuleAudio(Application* app, bool startEnabled = true);
	~ModuleAudio();

	bool Init();
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	uint LoadFx(const char* path);
	bool UnloadFx(uint index);

	// Play a previously loaded WAV
	int PlayFx(uint fx, int repeat = 0);
	int StopFx(int channel);

private:
	Mix_Music* music;
	p2List<Mix_Chunk*> fx;
};

#endif // __MODULE_AUDIO_H__