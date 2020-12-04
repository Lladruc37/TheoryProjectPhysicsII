#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

    // TITLE SCREEN //
    currentScene = TITLE_SCREEN;
    backgroundTitle = App->textures->Load("Assets/Textures/TitleScreen.png");
    //App->audio->PlayMusic("pinball/audio/music/TitleScreen.ogg", 0.0f);

    // GAME //
    startGame = false;
    backgroundGame = App->textures->Load("Assets/Textures/BG.png");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

update_status ModuleSceneIntro::PreUpdate()
{
    switch (currentScene)
    {
        case TITLE_SCREEN:
        {
            if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_STATE::KEY_DOWN) {
                currentScene = GAME;
                //App->audio->PlayMusic("pinball/audio/music/silence.ogg");
            }
            break;
        }
        case GAME:
        {
            if (startGame)
            {
                App->player->Enable();
                startGame = false;
            }
            break;
        }
    }
    return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::Update(float dt)
{
    switch (currentScene)
    {
        case TITLE_SCREEN:
        {
            break;
        }
        case GAME:
        {
            break;
        }
    }
    return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate()
{
    switch (currentScene)
    {
        case TITLE_SCREEN:
        {
            App->renderer->Blit(backgroundTitle, 0, 0,true);
            break;
        }
        case GAME:
        {
            App->renderer->Blit(backgroundGame, 0, 0, true);
            break;
        }
    }
    return UPDATE_CONTINUE;
}