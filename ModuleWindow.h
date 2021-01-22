#ifndef __MODULE_WINDOW_H__
#define __MODULE_WINDOW_H__

#include "Globals.h"
#include "Module.h"
#include "SDL/include/SDL.h"

class ModuleWindow : public Module
{
public:
	ModuleWindow(Application* app, bool startEnabled = true);
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screenSurface;
};

#endif // __MODULE_WINDOW_H__