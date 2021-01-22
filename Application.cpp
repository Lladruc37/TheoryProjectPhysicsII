#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"

Application::Application()
{
	PERF_START(pTimer);

	renderer = new ModuleRender(this);
	window = new ModuleWindow(this);
	textures = new ModuleTextures(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	player = new ModulePlayer(this);
	sceneIntro = new ModuleSceneIntro(this);
	physics = new ModulePhysics(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(input);
	AddModule(window);
	AddModule(physics);
	AddModule(textures);
	AddModule(audio);

	// Scenes
	AddModule(sceneIntro);

	// Player
	AddModule(player);

	// Renderer
	AddModule(renderer);

	PERF_PEEK(pTimer);
}

Application::~Application()
{
	p2List_item<Module*>* item = listModules.GetLast();

	while (item != NULL)
	{
		delete item->data;
		item = item->prev;
	}
}

bool Application::Init()
{
	PERF_START(pTimer);

	bool ret = true;

	if (cap > 0)
		cappedMs = 1000 / cap;

	// Call Init() in all modules
	p2List_item<Module*>* item = listModules.GetFirst();

	while (item != NULL && ret == true)
	{
		ret = item->data->Init();
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = listModules.GetFirst();

	while (item != NULL && ret == true)
	{
		if (item->data->IsEnabled())
			ret = item->data->Start();
		item = item->next;
	}

	PERF_PEEK(pTimer);

	return ret;
}

// Call PreUpdate, Update and PostUpdate on all modules
UpdateStatus Application::Update()
{
	UpdateStatus ret = UPDATE_CONTINUE;
	p2List_item<Module*>* item = listModules.GetFirst();
	PrepareUpdate();

	while (item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled())
			ret = item->data->PreUpdate();
		item = item->next;
	}

	item = listModules.GetFirst();

	while (item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled())
			ret = item->data->Update(dt);
		item = item->next;
	}

	item = listModules.GetFirst();

	while (item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled())
			ret = item->data->PostUpdate();
		item = item->next;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item = listModules.GetLast();

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	listModules.Add(mod);
}

void Application::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;

	// Calculate the dt: differential time since last frame
	dt = frameTime.ReadSec();

	// We start the timer after read because we want to know how much time it took from the last frame to the new one
	PERF_START(frameTime);
}

void Application::FinishUpdate()
{
	// Framerate calculations------------------------------------------
	// To know how many frames have passed in the last second
	if (lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
	}

	// Amount of seconds since startup
	float secondsSinceStartup = 0.0f;
	secondsSinceStartup = startupTime.ReadSec();

	// Amount of time since game start (use a low resolution timer)
	uint32 lastFrameMs = 0;
	lastFrameMs = frameTime.Read(); // Time from the prepare update until now (whole update method)

	// Average FPS for the whole game life (since start)
	float averageFps = 0.0f;
	averageFps = float(frameCount) / startupTime.ReadSec();

	// Amount of frames during the last update
	uint32 framesOnLastUpdate = 0;
	framesOnLastUpdate = prevLastSecFrameCount;

	static char title[256];

	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %I64u ",
		averageFps, lastFrameMs, framesOnLastUpdate, dt, secondsSinceStartup, frameCount);
	window->SetTitle(title);

	// Use SDL_Delay to make sure you get your capped framerate
	PERF_START(pTimer);
	if (cappedMs > lastFrameMs)
		SDL_Delay(cappedMs - lastFrameMs);

	// Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected
	//PERF_PEEK(ptimer);
}