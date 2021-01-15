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
    App->audio->PlayMusic("Assets/Sound/title.ogg", 0.0f);

    // GAME //
    startGame = false;
    currentScreen = GameScreen::EARTH;
    bgEarth = App->textures->Load("Assets/Textures/BG.png");
    bgAsteroids = App->textures->Load("Assets/Textures/BG2.png");
    bgMoon = App->textures->Load("Assets/Textures/BG3.png");

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
                startGame = true;
            }
            break;
        }
        case GAME:
        {
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
            if (startGame)
            {
                App->physics->Enable();
                App->player->Enable();
                currentScene = GAME;
                startGame = false;
            }
            App->renderer->Blit(backgroundTitle, 0, 0,true);
            break;
        }
        case GAME:
        {
            if ((App->player->position.y + App->player->height / 2) <= 0)
            {
                App->player->position.y = App->renderer->camera.h - App->player->height / 2;
                switch (currentScreen)
                {
                case EARTH:
                {
                    currentScreen = ASTEROIDS;
                    break;
                }
                case ASTEROIDS:
                {
                    currentScreen = MOON;
                    break;
                }
                }
            }
            else if (App->player->position.y >= App->renderer->camera.h)
            {
                App->player->position.y = 0;
                switch (currentScreen)
                {
                case ASTEROIDS:
                {
                    currentScreen = EARTH;
                    break;
                }
                case MOON:
                {
                    currentScreen = ASTEROIDS;
                    break;
                }
                }
            }

            switch (currentScreen)
            {
            case EARTH:
            {
                App->renderer->Blit(bgEarth, 0, 0, true);
                break;
            }
            case ASTEROIDS:
            {
                App->renderer->Blit(bgAsteroids, 0, 0, true);
                break;
            }
            case MOON:
            {
                App->renderer->Blit(bgMoon, 0, 0, true);
                break;
            }
            default:
            {
                break;
            }
            }
            break;
        }
    }
    return UPDATE_CONTINUE;
}