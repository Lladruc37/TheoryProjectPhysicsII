#ifndef __MODULE_TEXTURES_H__
#define __MODULE_TEXTURES_H__

#include "Module.h"
#include "Globals.h"
#include "SDL\include\SDL.h"

class ModuleTextures : public Module
{
public:
	ModuleTextures(Application* app, bool startEnabled = true);
	~ModuleTextures();

	bool Init();
	bool CleanUp();

	// Load new texture from file path
	SDL_Texture* const Load(const char* path);

	// Free texture from memory
	void Unload(SDL_Texture* texture);

public:
	p2List<SDL_Texture*> textures;
};

#endif // !__MODULE_TEXTURES_H__