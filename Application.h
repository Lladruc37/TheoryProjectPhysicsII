#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "p2List.h"
#include "Globals.h"
#include "Module.h"
#include "PerfTimer.h"
#include "Timer.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModulePlayer;
class ModuleSceneIntro;
class ModulePhysics;

class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModulePlayer* player;
	ModuleSceneIntro* sceneIntro;
	ModulePhysics* physics;

private:
	p2List<Module*> listModules;

	// Frame variables
	PerfTimer pTimer;
	uint64 frameCount = 0;

	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;
	uint32 lastSecFrameCount = 0;
	uint32 prevLastSecFrameCount = 0;
	float dt = 0.0f;
	float cappedMs = -1;
	int cap = 60;

public:
	Application();
	~Application();

	bool Init();
	UpdateStatus Update();
	bool CleanUp();

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

#endif // !__APPLICATION_H__