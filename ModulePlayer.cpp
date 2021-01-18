#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
    playerTex = App->textures->Load("Assets/Textures/Rocket.png");
    explosion = App->textures->Load("Assets/Textures/Explosion.png");
    //explosionAnim.totalFrames = 0;
    for (int i = 0; i != 10; ++i)
    {
        explosionAnim.PushBack({ i * 202,0,202,202 });
    }
    explosionAnim.loop = false;
    explosionAnim.speed = 15.0f;
    explosionAnim.Reset();

    player.mass = 2;
    width = 120;
    height = 56;
    angle = 0.0f;
    player.pos.x = 200;
    player.pos.y = 100;
    pastPos.x = player.pos.x;
    pastPos.y = player.pos.y;
    player.force.SetToZero();

    if (player.collider == nullptr)
    {
        player.collider = new Collider({ player.pos.x,player.pos.y,width,height }, Collider::Type::PLAYER, this);
    }

    App->physics->AddObject(&player);

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

    App->textures->Unload(playerTex);
    App->textures->Unload(explosion);
    App->audio->UnloadFx(movingFx);
    App->audio->UnloadFx(explosionFx);

	return true;
}

update_status ModulePlayer::PreUpdate()
{
    if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
    {
        isDestroyed = true;
        App->audio->PlayFx(explosionFx);
    }
    if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
    {
        godMode = !godMode;
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
            player.force.y = -1000.0f;
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
            player.force.x = -1000.0f;
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
            player.force.y = 1000.0f;
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
            player.force.x = 1000.0f;
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
            player.pos.x = 200;
            player.pos.y = 100;
            pastPos.x = player.pos.x;
            pastPos.y = player.pos.y;
            player.force.SetToZero();
            isMovingUp = false;
            isMovingLeft = false;
            isMovingRight = false;
            isMovingDown = false;
            isDestroyed = false;
            explosionAnim.Reset();
            if (player.collider == nullptr)
            {
                player.collider = new Collider({ player.pos.x,player.pos.y,width,height }, Collider::Type::PLAYER, this);
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
        /*
        nextPos = position;
        nextSpeed = speed;
        fPoint a = App->physics->Force2Accel(totalForce, mass);
        App->physics->UpdatePhysics(nextPos, nextSpeed, a, dt);
        App->physics->ResolveCollisions(position, nextPos, speed, nextSpeed, width, height);
        collider->SetPos(position.x, position.y, width, height);
        */
        pastPos = player.pos;
        pastSpeed = player.speed;
        //App->physics->UpdatePhysics(&player, dt);
        App->physics->ResolveCollisions(&player, pastPos, pastSpeed, width, height);
        //player.collider->SetPos(player.pos.x, player.pos.y, width, height);

        // Player boundries
        if (player.pos.x <= 0) //Left bound
        {
            player.pos.x = 1;
            //speed.x = 0.0f;
            //a.x = 0.0f;
        }
        if ((player.pos.x + width) > (App->renderer->camera.w)) //Right bound
        {
            player.pos.x = App->renderer->camera.w - width - 1;
            //speed.x = 0.0f;
            //a.x = 0.0f;
        }
        if (player.pos.y <= 0 && App->scene_intro->currentScreen == GameScreen::MOON) //Up bound
        {
            player.pos.y = 1;
            //speed.y = 0.0f;
            //a.y = 0.0f;
        }
        if ((player.pos.y + height) > (App->renderer->camera.h) && App->scene_intro->currentScreen == GameScreen::EARTH) //Bottom bound
        {
            player.pos.y = App->renderer->camera.h - height - 1;
            //speed.y = 0.0f;
            //a.y = 0.0f;
        }

        if (player.speed.y > 200.0f)
            player.speed.y = 200.0f;
        else if (player.speed.y < -200.0f)
            player.speed.y = -200.0f;
        if (player.speed.x > 200.0f)
            player.speed.x = 200.0f;
        else if (player.speed.x < -200.0f)
            player.speed.x = -200.0f;
    }
    else
    {
        if (isDestroyed)
        {
            if (player.collider != nullptr)
            {
                player.collider->pendingToDelete = true;
                player.collider = nullptr;
            }
            App->scene_intro->DeleteAsteroids();
            explosionAnim.Update(dt);
            player.speed.x = 0.0f;
            player.speed.y = 0.0f;
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
        App->renderer->Blit(explosion, player.pos.x - 41, player.pos.y - 73, false, 0, &explosionAnim.GetCurrentFrame());
    }
    else
    {
        App->renderer->Blit(playerTex, player.pos.x, player.pos.y, false, (double)angle);
    }
    
    if (App->physics->debug && player.collider != nullptr)
        App->renderer->DrawQuad(player.collider->rect, 0, 255, 0, 100);

    return UPDATE_CONTINUE;
};

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
    if (!godMode)
    {
        if (App->scene_intro->currentScreen == GameScreen::ASTEROIDS)
        {
            if (c2->type == Collider::Type::ASTEROID)
            {
                isDestroyed = true;
                App->audio->PlayFx(explosionFx);
            }
        }
        if (App->scene_intro->currentScreen == GameScreen::MOON)
        {
            if (c2->type == Collider::Type::MOON)
            {
                LOG("YAY");
            }
        }
    }
}
