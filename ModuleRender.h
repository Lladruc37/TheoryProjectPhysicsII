#ifndef __MODULE_RENDER_H__
#define __MODULE_RENDER_H__

#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL.h"

class ModuleRender : public Module
{
public:
	ModuleRender(Application* app, bool startEnabled = true);
	~ModuleRender();

	bool Init();
	UpdateStatus PreUpdate();
	UpdateStatus PostUpdate();
	bool CleanUp();

	bool Blit(SDL_Texture* texture, int x, int y, bool fullscreen, double angle = 0, SDL_Rect* section = NULL, float speed = 1.0f, int pivotX = INT_MAX, int pivotY = INT_MAX);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool useCamera = true);
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true);
	bool DrawCircle(int x1, int y1, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool useCamera = true);
	bool FillCircle(int x1, int y1, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true);

public:
	SDL_Renderer* renderer;
	SDL_Rect camera;
};

#endif // !__MODULE_RENDER_H__