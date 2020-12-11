#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
    player = App->textures->Load("Assets/Textures/Rocket.png");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

update_status ModulePlayer::PreUpdate()
{
    if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
    {
        force.y = 5.0f;
    }

    if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
    {
        force.x = -5.0f;
    }

    if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
    {
        force.y = -5.0f;
    }

    if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
    {
        force.x = 5.0f;
    }
    return UPDATE_CONTINUE;
};

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{
    App->renderer->Blit(player, 0, 0, false);
    return UPDATE_CONTINUE;
};



