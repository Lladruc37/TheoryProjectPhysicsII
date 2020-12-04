#pragma once
#include "Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "Globals.h"

enum Scene
{
    TITLE_SCREEN,
    GAME,
    GAME_OVER
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
    update_status PreUpdate();
	update_status Update(float dt);
    update_status PostUpdate();
	bool CleanUp();

public:
    Scene currentScene = TITLE_SCREEN;

    // GAME //
    SDL_Texture* backgroundGame = nullptr;
    bool startGame = false;

    // TITLE SCREEN //
    SDL_Texture* backgroundTitle = nullptr;
};
