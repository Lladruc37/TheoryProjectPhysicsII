#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Module.h"
#include "PerfTimer.h"
#include "Timer.h"

#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"

class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModulePlayer* player;
	ModuleSceneIntro* scene_intro;
	ModulePhysics* physics;

private:

	p2List<Module*> list_modules;

    // Frame variables
    PerfTimer ptimer;
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
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
    void PrepareUpdate();
    void FinishUpdate();
};