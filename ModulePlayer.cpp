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
    mass = 2;
    width = 101;
    height = 273;
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
        totalForce.y = -12500.0f;
    }

    if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
    {
        totalForce.x = -10000.0f;
    }

    if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
    {
        totalForce.y = 10000.0f;
    }

    if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
    {
        totalForce.x = 10000.0f;
    }
    return UPDATE_CONTINUE;
};

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
    nextPos = position;
    nextSpeed = speed;
    fPoint a = App->physics->Force2Accel(totalForce, mass);
    App->physics->UpdatePhysics(nextPos, nextSpeed, a, dt);
    App->physics->ResolveCollisions(position, nextPos, speed, nextSpeed, width, height);
	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{
    App->renderer->Blit(player, position.x, position.y, false);
    return UPDATE_CONTINUE;
};



