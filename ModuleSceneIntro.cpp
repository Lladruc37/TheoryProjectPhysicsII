#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include <time.h>


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

    srand(time(NULL));
    asteroidTxt = App->textures->Load("Assets/Textures/Asteroid.png");

    // WIN/LOSE //
    gameOverTxt = App->textures->Load("Assets/Textures/GameOver.png");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
    App->textures->Unload(backgroundTitle);
    App->textures->Unload(bgEarth);
    App->textures->Unload(bgAsteroids);
    App->textures->Unload(bgMoon);
    App->textures->Unload(asteroidTxt);

	return true;
}

update_status ModuleSceneIntro::PreUpdate()
{
    switch (currentScene)
    {
        case TITLE_SCREEN:
        {
            if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_STATE::KEY_DOWN)
            {
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
            if (currentScreen == ASTEROIDS)
            {
                //fPoint a;
                //a.x = 0;
                //a.y = 0;
                //App->physics->UpdatePhysics(top.pos, top.speed, a, dt);
                //App->physics->UpdatePhysics(&top, dt);
                if (top.pos.x >= App->renderer->camera.w + top.width)
                    top.pos.x = 0 - top.width;
                top.collider->SetPos(top.pos.x, top.pos.y, top.width, top.height);
                top.angle += 150.0f * dt;

                for (int i = 0; i != 3; ++i)
                {
                    //App->physics->UpdatePhysics(mid[i].pos, mid[i].speed, a, dt);
                    //App->physics->UpdatePhysics(&mid[i], dt);
                    if (mid[i].pos.x <= 0 - mid[i].width)
                        mid[i].pos.x = App->renderer->camera.w + mid[i].width;
                    mid[i].collider->SetPos(mid[i].pos.x, mid[i].pos.y, mid[i].width, mid[i].height);
                    mid[i].angle -= 100.0f * dt;
                }

                for (int i = 0; i != 4; ++i)
                {
                    //LOG("SpeedAsteroidX%d = %f",i, bot[i].speed.x);               Theres a bug with a strange increase of speed when asteroids are tpd
                    //App->physics->UpdatePhysics(bot[i].pos, bot[i].speed, a, dt);
                    //App->physics->UpdatePhysics(&bot[i], dt);
                    if (bot[i].pos.x >= App->renderer->camera.w + bot[i].width)
                        bot[i].pos.x = 0 - bot[i].width;
                    bot[i].collider->SetPos(bot[i].pos.x, bot[i].pos.y, bot[i].width, bot[i].height);
                    bot[i].angle += 50.0f * dt;
                }
            }
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
                currentScreen = EARTH;
                startGame = false;
            }
            App->renderer->Blit(backgroundTitle, 0, 0,true);
            break;
        }
        case GAME:
        {
            if ((App->player->player.pos.y + App->player->height / 2) <= 0)
            {
                App->player->player.pos.y = App->renderer->camera.h - App->player->height / 2;
                switch (currentScreen)
                {
                    case EARTH:
                    {
                        currentScreen = ASTEROIDS;
                        App->physics->gravity.x = 0.0f;
                        App->physics->gravity.y = 0.0f;
                        CreateAsteroids();
                        break;
                    }
                    case ASTEROIDS:
                    {
                        currentScreen = MOON;
                        App->physics->gravity.x = 0.0f;
                        App->physics->gravity.y = 0.0f;
                        break;
                    }
                }
            }
            else if (App->player->player.pos.y >= App->renderer->camera.h)
            {
                App->player->player.pos.y = 0;
                switch (currentScreen)
                {
                    case ASTEROIDS:
                    {
                        currentScreen = EARTH;
                        App->physics->gravity.x = 0.0f;
                        App->physics->gravity.y = 60.0f;
                        break;
                    }
                    case MOON:
                    {
                        currentScreen = ASTEROIDS;
                        App->physics->gravity.x = 0.0f;
                        App->physics->gravity.y = 0.0f;
                        CreateAsteroids();
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
                    App->renderer->Blit(asteroidTxt, top.pos.x, top.pos.y, false, top.angle);
                    for (int i = 0; i != 3; ++i)
                    App->renderer->Blit(asteroidTxt, mid[i].pos.x, mid[i].pos.y, false, mid[i].angle);
                    for (int i = 0; i != 5; ++i)
                    App->renderer->Blit(asteroidTxt, bot[i].pos.x, bot[i].pos.y, false, bot[i].angle);

                    if (App->physics->debug)
                    {
                        App->renderer->DrawQuad(top.collider->rect, 255, 0, 0, 100);
                        for (int i = 0; i != 3; ++i)
                        {
                            App->renderer->DrawQuad(mid[i].collider->rect, 255, 0, 0, 100);
                        }
                        for (int i = 0; i != 4; ++i)
                        {
                            App->renderer->DrawQuad(bot[i].collider->rect, 255, 0, 0, 100);
                        }
                    }
                    break;
                }
                case MOON:
                {
                    App->renderer->Blit(bgMoon, 0, 0, true);
                    break;
                }
            }

            if (App->player->isDestroyed)
            {
                App->renderer->Blit(gameOverTxt, 0, App->renderer->camera.h / 2 - 90, false);
            }
            break;
        }
    }
    return UPDATE_CONTINUE;
}

void ModuleSceneIntro::CreateAsteroids()
{
    top.width = top.height = 70;
    top.pos.x = -top.width;
    top.pos.y = 135;
    top.angle = (float)(rand() % 360);
    top.speed.x = 550.0f;
    top.mass = 3;
    if (top.collider == nullptr)
    {
        top.collider = new Collider({ top.pos.x,top.pos.y,top.width,top.height }, Collider::Type::ASTEROID, this);
        App->physics->AddObject(&top);
    }
    else
    {
        top.collider->SetPos(top.pos.x, top.pos.y, top.width, top.height);
    }

    for (int i = 0; i != 3; ++i)
    {
        mid[i].width = mid[i].height = 70;
        mid[i].pos.x = App->renderer->camera.w + mid[i].width - (i * 350);
        mid[i].pos.y = 375;
        mid[i].angle = (float)(rand() % 360);
        mid[i].speed.x = -300.0f;
        mid[i].mass = 3;
        if (mid[i].collider == nullptr)
        {
            mid[i].collider = new Collider({ mid[i].pos.x,mid[i].pos.y,mid[i].width,mid[i].height }, Collider::Type::ASTEROID, this);
            App->physics->AddObject(&mid[i]);
        }
        else
        {
            mid[i].collider->SetPos(mid[i].pos.x, mid[i].pos.y, mid[i].width, mid[i].height);
        }
    }

    for (int i = 0; i != 4; ++i)
    {
        bot[i].width = bot[i].height = 70;
        bot[i].pos.x = bot[i].width + (i * 262);
        bot[i].pos.y = 615;
        bot[i].angle = (float)(rand() % 360);
        bot[i].speed.x = 165.0f;
        bot[i].mass = 3;
        if (bot[i].collider == nullptr)
        {
            bot[i].collider = new Collider({ bot[i].pos.x,bot[i].pos.y,bot[i].width,bot[i].height }, Collider::Type::ASTEROID, this);
            App->physics->AddObject(&bot[i]);
        }
        else
        {
            bot[i].collider->SetPos(bot[i].pos.x, bot[i].pos.y, bot[i].width, bot[i].height);
        }
    }
}

void ModuleSceneIntro::OnCollision(Collider* c1, Collider* c2) {}