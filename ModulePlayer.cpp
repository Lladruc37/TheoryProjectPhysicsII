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
    width = 51;
    height = 137;
    position.x = 200;
    position.y = 100;
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
    if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
    {
        if ((position.y + height) > (App->renderer->camera.h))
        {
            totalForce.y = -100000.0f;
        }
        else
        {
            totalForce.y = -1000.0f;
        }
    }

    if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
    {
        if ((position.x + width) > (App->renderer->camera.w))
        {
            totalForce.x = -25000.0f;
        }
        else
        {
            totalForce.x = -250.0f;
        }
    }

    if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
    {
        if (position.y <= 0)
        {
            totalForce.y = 25000.0f;
        }
        else
        {
            totalForce.y = 250.0f;
        }
    }

    if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
    {
        if (position.x <= 0)
        {
            totalForce.x = 25000.0f;
        }
        else
        {
            totalForce.x = 250.0f;
        }
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

    // Car boundries
    if (position.x <= 0)
    {
        position.x = 0;
        speed.x = 0.0f;
        a.x = 0.0f;
    }
    if ((position.x + width) > (App->renderer->camera.w))
    {
        position.x = App->renderer->camera.w - width;
        speed.x = 0.0f;
        a.x = 0.0f;
    }
    if (position.y <= 0)
    {
        position.y = 0;
        speed.y = 0.0f;
        a.y = 0.0f;
    }
    if ((position.y + height) > (App->renderer->camera.h))
    {
        position.y = App->renderer->camera.h - height;
        speed.y = 0.0f;
        a.y = 0.0f;
    }
	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{
    App->renderer->Blit(player, position.x, position.y, false);
    return UPDATE_CONTINUE;
};



