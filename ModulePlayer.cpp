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
    explosion = App->textures->Load("Assets/Textures/Explosion.png");
    explosionAnim.totalFrames = 0;
    for (int i = 0; i != 10; ++i)
    {
        explosionAnim.PushBack({ i * 202,0,202,202 });
    }
    explosionAnim.loop = false;
    explosionAnim.speed = 15.0f;
    explosionAnim.Reset();

    mass = 2;
    width = 120;
    height = 56;
    angle = 0.0f;
    position.x = 200;
    position.y = 100;
    nextPos.x = position.x;
    nextPos.y = position.y;
    totalForce.SetToZero();

    if (collider == nullptr)
    {
        collider = App->physics->AddCollider({ position.x,position.y,width,height }, Collider::Type::PLAYER, this);
    }

    isMovingUp = false;
    isMovingLeft = false;
    isMovingRight = false;
    isMovingDown = false;
    isDestroyed = false;

    movingFx = App->audio->LoadFx("Assets/Sound/Fx/ovni.wav");
    explosionFx = App->audio->LoadFx("Assets/Sound/Fx/expurosion.wav");

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

    App->textures->Unload(player);
    App->textures->Unload(explosion);
    App->audio->UnloadFx(movingFx);
    App->audio->UnloadFx(explosionFx);

	return true;
}

update_status ModulePlayer::PreUpdate()
{
    totalForce.x = 0;
    totalForce.y = 0;
    if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
    {
        isDestroyed = true;
        App->audio->PlayFx(explosionFx);
    }

    if (!isDestroyed)
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
    }
    else
    {
        if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
        {
            position.x = 200;
            position.y = 100;
            nextPos.x = position.x;
            nextPos.y = position.y;
            totalForce.SetToZero();
            isMovingUp = false;
            isMovingLeft = false;
            isMovingRight = false;
            isMovingDown = false;
            isDestroyed = false;
            explosionAnim.Reset();
            if (collider == nullptr)
            {
                collider = App->physics->AddCollider({ position.x,position.y,width,height }, Collider::Type::PLAYER, this);
            }
            App->scene_intro->currentScreen = GameScreen::EARTH;
        }
    }

    return UPDATE_CONTINUE;
};

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
    dtAnim = dt;
    if (!isDestroyed)
    {
        nextPos = position;
        nextSpeed = speed;
        fPoint a = App->physics->Force2Accel(totalForce, mass);
        App->physics->UpdatePhysics(nextPos, nextSpeed, a, dt);
        App->physics->ResolveCollisions(position, nextPos, speed, nextSpeed, width, height);
        collider->SetPos(position.x, position.y, width, height);

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

        if (speed.y > 200.0f)
            speed.y = 200.0f;
        else if (speed.y < -200.0f)
            speed.y = -200.0f;
        if (speed.x > 200.0f)
            speed.x = 200.0f;
        else if (speed.x < -200.0f)
            speed.x = -200.0f;
    }
    else
    {
        if (isDestroyed)
        {
            if (collider != nullptr)
            {
                collider->pendingToDelete = true;
                collider = nullptr;
            }
            explosionAnim.Update(dt);
        }
        /*else                  Victory
        {

        }*/
    }
    return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{
    //SDL_Rect hitbox = { position.x, position.y, width, height };

    if (isDestroyed)
    {
        if(!explosionAnim.HasFinished())
        App->renderer->Blit(explosion, position.x - 41, position.y - 73, false, 0, &explosionAnim.GetCurrentFrame());
    }
    else
    {
        App->renderer->Blit(player, position.x, position.y, false, (double)angle);
    }
    
    if (App->physics->debug && collider != nullptr)
        App->renderer->DrawQuad(collider->rect, 0, 255, 0, 100);

    return UPDATE_CONTINUE;
};

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
    if (App->scene_intro->currentScreen == GameScreen::ASTEROIDS)
    {
        if (c1->type == Collider::Type::ASTEROID || c2->type == Collider::Type::ASTEROID)   //Rn like this cause im not sure which is which
        {
            isDestroyed = true;
            App->audio->PlayFx(explosionFx);
        }
    }
}
