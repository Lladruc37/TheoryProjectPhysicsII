#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"

ModuleRender::ModuleRender(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	renderer = NULL;
	camera.x = camera.y = 0;
	camera.w = SCREEN_WIDTH;
	camera.h = SCREEN_HEIGHT;
}

ModuleRender::~ModuleRender()
{}

bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	if (VSYNC == true)
		flags |= SDL_RENDERER_PRESENTVSYNC;

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);

	if (renderer == NULL)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

UpdateStatus ModuleRender::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

UpdateStatus ModuleRender::PostUpdate()
{
	SDL_RenderPresent(renderer);
	return UPDATE_CONTINUE;
}

bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	if (renderer != NULL)
		SDL_DestroyRenderer(renderer);

	return true;
}

bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, bool fullscreen, double angle, SDL_Rect* section, float speed, int pivotX, int pivotY)
{
	bool ret = true;
	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * SCREEN_SIZE;
	rect.y = (int)(camera.y * speed) + y * SCREEN_SIZE;

	if (fullscreen == true)
	{
		rect.w = (int)SCREEN_WIDTH;
		rect.h = (int)SCREEN_HEIGHT;
	}
	else
	{
		if (section != NULL)
		{
			rect.w = section->w;
			rect.h = section->h;
		}
		else
		{
			SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
		}
	}

	rect.w *= SCREEN_SIZE;
	rect.h *= SCREEN_SIZE;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool useCamera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (useCamera)
	{
		rec.x = (int)(camera.x + rect.x * SCREEN_SIZE);
		rec.y = (int)(camera.y + rect.y * SCREEN_SIZE);
		rec.w *= SCREEN_SIZE;
		rec.h *= SCREEN_SIZE;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool useCamera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if (useCamera)
	{
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * SCREEN_SIZE, camera.y + y1 * SCREEN_SIZE, camera.x + x2 * SCREEN_SIZE, camera.y + y2 * SCREEN_SIZE);
	}
	else
	{
		result = SDL_RenderDrawLine(renderer, x1 * SCREEN_SIZE, y1 * SCREEN_SIZE, x2 * SCREEN_SIZE, y2 * SCREEN_SIZE);
	}

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool useCamera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for (uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if (result != 0)
	{
		LOG("Cannot draw circle to screen. SDL_RenderDrawPoints error: %s", SDL_GetError());
		ret = false;
	}

	if (filled)
	{
		return FillCircle(x, y, radius, r, g, b, a, useCamera);
	}
	else
	{
		return ret;
	}
}

bool ModuleRender::FillCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool useCamera)
{
	int offsetx, offsety, d;
	bool status;

	offsetx = 0;
	offsety = radius;
	d = radius - 1;
	status = 0;

	while (offsety >= offsetx)
	{
		status = DrawLine(x - offsety, y + offsetx, x + offsety, y + offsetx, r, g, b, a, useCamera);
		status = DrawLine(x - offsetx, y + offsety, x + offsetx, y + offsety, r, g, b, a, useCamera);
		status = DrawLine(x - offsetx, y - offsety, x + offsetx, y - offsety, r, g, b, a, useCamera);
		status = DrawLine(x - offsety, y - offsetx, x + offsety, y - offsetx, r, g, b, a, useCamera);

		if (status == false)
			break;

		if (d >= 2 * offsetx)
		{
			d -= 2 * offsetx + 1;
			offsetx += 1;
		}
		else if (d < 2 * (radius - offsety))
		{
			d += 2 * offsety - 1;
			offsety -= 1;
		}
		else
		{
			d += 2 * (offsety - offsetx - 1);
			offsety -= 1;
			offsetx += 1;
		}
	}
	return status;
}