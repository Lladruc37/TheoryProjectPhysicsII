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
    width = 120;
    height = 56;
    angle = 0.0f;
    position.x = 200;
    position.y = 100;
    nextPos.x = position.x;
    nextPos.y = position.y;

    isMovingUp = false;
    isMovingLeft = false;
    isMovingRight = false;
    isMovingDown = false;

    movingFx = App->audio->LoadFx("Assets/Sound/Fx/ovni.wav");

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
    if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
    {
        angle -= 1.0f;
    }
    if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
    {
        angle += 1.0f;
    }
    if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
    {
        //if ((position.y + height) > (App->renderer->camera.h))
        //{
            totalForce.y = -1000.0f;
        //}
        //else
        /*{
            totalForce.y = -1000.0f;
        }*/

        if (!isMovingUp)
        {
            isMovingUp = true;
            movingChannelOne = App->audio->PlayFx(movingFx, -1);
        }
    }
    else
    {
        if (isMovingUp)
        {
            isMovingUp = false;
            App->audio->StopFx(movingChannelOne);
        }
    }

    if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
    {
        /*if ((position.x + width) > (App->renderer->camera.w))
        {
            totalForce.x = -25000.0f;
        }
        else
        {*/
            totalForce.x = -1000.0f;
        //}

        if (!isMovingLeft)
        {
            isMovingLeft = true;
            movingChannelTwo = App->audio->PlayFx(movingFx, -1);
        }
    }
    else
    {
        if (isMovingLeft)
        {
            isMovingLeft = false;
            App->audio->StopFx(movingChannelTwo);
        }
    }

    if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
    {
        //if (position.y <= 0)
        //{
            totalForce.y = 1000.0f;
        //}
        /*else
        {
            totalForce.y = 250.0f;
        }*/

        if (!isMovingRight)
        {
            isMovingRight = true;
            movingChannelThree = App->audio->PlayFx(movingFx, -1);
        }
    }
    else
    {
        if (isMovingRight)
        {
            isMovingRight = false;
            App->audio->StopFx(movingChannelThree);
        }
    }

    if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
    {
        /*if (position.x <= 0)
        {
            totalForce.x = 25000.0f;
        }
        else
        {*/
            totalForce.x = 1000.0f;
        //}

        if (!isMovingDown)
        {
            isMovingDown = true;
            movingChannelFour = App->audio->PlayFx(movingFx, -1);
        }
    }
    else
    {
        if (isMovingDown)
        {
            isMovingDown = false;
            App->audio->StopFx(movingChannelFour);
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
    if (position.x <= 0) //Left bound
    {
        position.x = 1;
        //speed.x = 0.0f;
        a.x = 0.0f;
    }
    if ((position.x + width) > (App->renderer->camera.w)) //Right bound
    {
        position.x = App->renderer->camera.w - width - 1;
        //speed.x = 0.0f;
        a.x = 0.0f;
    }
    if (position.y <= 0 && App->scene_intro->currentScreen == GameScreen::MOON) //Up bound
    {
        position.y = 1;
        //speed.y = 0.0f;
        a.y = 0.0f;
    }
    if ((position.y + height) > (App->renderer->camera.h) && App->scene_intro->currentScreen == GameScreen::EARTH) //Bottom bound
    {
        position.y = App->renderer->camera.h - height - 1;
        //speed.y = 0.0f;
        a.y = 0.0f;
    }
	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{
    App->renderer->Blit(player, position.x, position.y, false, (double)angle);
    return UPDATE_CONTINUE;
};



