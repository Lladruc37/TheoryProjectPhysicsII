#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"
#include <time.h>


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool startEnabled) : Module(app, startEnabled)
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
    bgTitle = App->textures->Load("Game/Assets/Textures/TitleScreen.png");
    App->audio->PlayMusic("Game/Assets/Sound/title.ogg", 0.0f);

    // GAME //
    startGame = false;
    currentScreen = GameScreen::EARTH;
    bgEarth = App->textures->Load("Game/Assets/Textures/BG.png");
    bgAsteroids = App->textures->Load("Game/Assets/Textures/BG2.png");
    bgMoon = App->textures->Load("Game/Assets/Textures/BG3.png");
    bgWater = App->textures->Load("Game/Assets/Textures/BG4.png");
    flagTxt = App->textures->Load("Game/Assets/Textures/Flag.png");
    asteroidTxt = App->textures->Load("Game/Assets/Textures/Asteroid.png");

    srand(time(NULL));

    // WIN/LOSE //
    gameOverTxt = App->textures->Load("Game/Assets/Textures/GameOver.png");
    victoryTxt = App->textures->Load("Game/Assets/Textures/MissionComplete.png");

    return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
    LOG("Unloading Intro scene");
    App->textures->Unload(bgTitle);
    App->textures->Unload(bgEarth);
    App->textures->Unload(bgAsteroids);
    App->textures->Unload(bgMoon);
    App->textures->Unload(asteroidTxt);
    App->textures->Unload(flagTxt);

    return true;
}

UpdateStatus ModuleSceneIntro::PreUpdate()
{
    switch (currentScene)
    {
    case TITLE_SCREEN:
        if (App->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
            startGame = true;
        break;

    case GAME:
        if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
            App->physics->debug = !App->physics->debug;

        if ((App->player->player.pos.y + App->player->player.radius / 2) <= 0)
        {
            App->player->player.pos.y = App->renderer->camera.h - App->player->player.radius / 2;
            switch (currentScreen)
            {
            case EARTH:
                currentScreen = ASTEROIDS;
                DeleteEarth();
                CreateAsteroids();
                break;

            case ASTEROIDS:
                currentScreen = MOON;
                DeleteAsteroids();
                CreateMoon();
                break;
            }
        }
        else if (App->player->player.pos.y >= App->renderer->camera.h)
        {
            App->player->player.pos.y = 0;
            switch (currentScreen)
            {
            case ASTEROIDS:
                currentScreen = EARTH;
                DeleteAsteroids();
                CreateEarth(App->player->checkpoint);
                break;

            case MOON:
                currentScreen = ASTEROIDS;
                DeleteMoon();
                CreateAsteroids();
                break;
            }
        }
        break;
    }
    return UPDATE_CONTINUE;
}

UpdateStatus ModuleSceneIntro::Update(float dt)
{
    switch (currentScene)
    {
    case TITLE_SCREEN:
        break;

    case GAME:
        if (currentScreen == ASTEROIDS)
        {
            if (top.collider != nullptr)
            {
                if (top.pos.x >= App->renderer->camera.w + top.collider->rect.w)
                    top.pos.x = 0 - top.collider->rect.w;
            }
            top.angle += 150.0f * dt;

            for (int i = 0; i != 3; ++i)
            {
                if (mid[i].collider != nullptr)
                {
                    if (mid[i].pos.x <= 0 - mid[i].collider->rect.w)
                        mid[i].pos.x = App->renderer->camera.w + mid[i].collider->rect.w;
                }
                mid[i].angle -= 100.0f * dt;
            }

            for (int i = 0; i != 4; ++i)
            {
                if (bot[i].collider != nullptr)
                {
                    if (bot[i].pos.x >= App->renderer->camera.w + bot[i].collider->rect.w)
                        bot[i].pos.x = 0 - bot[i].collider->rect.w;
                }
                bot[i].angle += 50.0f * dt;
            }
        }
        break;
    }
    return UPDATE_CONTINUE;
}

UpdateStatus ModuleSceneIntro::PostUpdate()
{
    switch (currentScene)
    {
    case TITLE_SCREEN:
        if (startGame)
        {
            App->physics->Enable();
            App->player->Enable();
            currentScene = GAME;
            currentScreen = EARTH;
            CreateEarth(App->player->checkpoint);
            startGame = false;
        }
        App->renderer->Blit(bgTitle, 0, 0, true);
        break;

    case GAME:
        switch (currentScreen)
        {
        case EARTH:
            if (!App->player->checkpoint)
                App->renderer->Blit(bgEarth, 0, 0, true);
            else
                App->renderer->Blit(bgWater, 0, 0, true);

            if (App->physics->debug && land.collider != nullptr)
                App->renderer->DrawQuad(land.collider->rect, 255, 0, 0, 100);

            if (App->physics->debug && water.collider != nullptr)
                App->renderer->DrawQuad(water.collider->rect, 0, 0, 255, 100);

            break;

        case ASTEROIDS:
            App->renderer->Blit(bgAsteroids, 0, 0, true);
            App->renderer->Blit(asteroidTxt, top.pos.x - top.radius, top.pos.y - top.radius, false, top.angle);

            for (int i = 0; i != 3; ++i)
                App->renderer->Blit(asteroidTxt, mid[i].pos.x - mid[i].radius, mid[i].pos.y - mid[i].radius, false, mid[i].angle);

            for (int i = 0; i != 4; ++i)
                App->renderer->Blit(asteroidTxt, bot[i].pos.x - bot[i].radius, bot[i].pos.y - bot[i].radius, false, bot[i].angle);

            if (App->physics->debug)
            {
                if (top.collider != nullptr)
                    App->renderer->DrawCircle(top.pos.x, top.pos.y, top.radius, 255, 0, 0, 100);

                for (int i = 0; i != 3; ++i)
                {
                    if (mid[i].collider != nullptr)
                        App->renderer->DrawCircle(mid[i].pos.x, mid[i].pos.y, mid[i].radius, 255, 0, 0, 100);
                }
                for (int i = 0; i != 4; ++i)
                {
                    if (bot[i].collider != nullptr)
                        App->renderer->DrawCircle(bot[i].pos.x, bot[i].pos.y, bot[i].radius, 255, 0, 0, 100);
                }
            }
            break;

        case MOON:
            App->renderer->Blit(bgMoon, 0, 0, true);

            if (App->physics->debug)
                App->renderer->DrawCircle(moon.pos.x, moon.pos.y, moon.radius, 255, 0, 0, 100);

            if (App->player->checkpoint)
                App->renderer->Blit(flagTxt, App->player->flagPosition.x, App->player->flagPosition.y, false, App->player->flagAngle);
            break;
        }

        if (App->player->isDestroyed)
            App->renderer->Blit(gameOverTxt, 0, App->renderer->camera.h / 2 - 90, false);
        else if (App->player->victory)
            App->renderer->Blit(victoryTxt, 0, App->renderer->camera.h / 2 - 160, false);
        break;
    }
    return UPDATE_CONTINUE;
}

void ModuleSceneIntro::CreateAsteroids()
{
    top.mass = 1.0f;
    top.radius = 35;
    top.pos.x = -70;
    top.pos.y = 160;
    top.pastPos.x = top.pos.x;
    top.pastPos.y = top.pos.y;
    top.speed.x = 550.0f;
    top.angle = (float)(rand() % 360);
    top.shape = Object::Shape::CIRCLE;
    if (top.collider == nullptr)
    {
        top.collider = new Collider({ top.pos.x - top.radius,top.pos.y - top.radius,top.radius * 2,top.radius * 2 }, Collider::Type::ASTEROID, this);
        App->physics->AddObject(&top);
    }

    for (int i = 0; i != 3; ++i)
    {
        mid[i].mass = 1.0f;
        mid[i].radius = 35;
        mid[i].pos.x = App->renderer->camera.w + 105 - (i * 350);
        mid[i].pos.y = 435;
        mid[i].pastPos.x = mid[i].pos.x;
        mid[i].pastPos.y = mid[i].pos.y;
        mid[i].speed.x = -300.0f;
        mid[i].angle = (float)(rand() % 360);
        mid[i].shape = Object::Shape::CIRCLE;
        if (mid[i].collider == nullptr)
        {
            mid[i].collider = new Collider({ mid[i].pos.x - mid[i].radius,mid[i].pos.y - mid[i].radius,mid[i].radius * 2,mid[i].radius * 2 }, Collider::Type::ASTEROID, this);
            App->physics->AddObject(&mid[i]);
        }
    }

    for (int i = 0; i != 4; ++i)
    {
        bot[i].mass = 1.0f;
        bot[i].radius = 35;
        bot[i].pos.x = 105 + (i * 262);
        bot[i].pos.y = 675;
        bot[i].pastPos.x = bot[i].pos.x;
        bot[i].pastPos.y = bot[i].pos.y;
        bot[i].speed.x = 165.0f;
        bot[i].angle = (float)(rand() % 360);
        bot[i].shape = Object::Shape::CIRCLE;
        if (bot[i].collider == nullptr)
        {
            bot[i].collider = new Collider({ bot[i].pos.x - bot[i].radius,bot[i].pos.y - bot[i].radius,bot[i].radius * 2,bot[i].radius * 2 }, Collider::Type::ASTEROID, this);
            App->physics->AddObject(&bot[i]);
        }
    }
}

void ModuleSceneIntro::DeleteAsteroids()
{
    if (top.collider != nullptr)
        App->physics->RemoveObject(&top);

    for (int i = 0; i != 3; ++i)
    {
        if (mid[i].collider != nullptr)
            App->physics->RemoveObject(&mid[i]);
    }

    for (int i = 0; i != 4; ++i)
    {
        if (bot[i].collider != nullptr)
            App->physics->RemoveObject(&bot[i]);
    }
}

void ModuleSceneIntro::CreateMoon()
{
    moon.mass = 0.0f;
    moon.radius = 186;
    moon.pos.x = 461;
    moon.pos.y = 245;
    moon.pastPos.x = moon.pos.x;
    moon.pastPos.y = moon.pos.y;
    moon.shape = Object::Shape::CIRCLE;
    if (moon.collider == nullptr)
    {
        moon.collider = new Collider({ moon.pos.x - moon.radius,moon.pos.y - moon.radius,moon.radius * 2,moon.radius * 2 }, Collider::Type::MOON, this);
        App->physics->AddObject(&moon);
    }
}

void ModuleSceneIntro::DeleteMoon()
{
    if (moon.collider != nullptr)
        App->physics->RemoveObject(&moon);
}

void ModuleSceneIntro::CreateEarth(bool checkpoint)
{
    if (!checkpoint)
    {
        land.mass = 0.0f;
        land.pos.x = 0;
        land.pos.y = 729;
        land.pastPos.x = land.pos.x;
        land.pastPos.y = land.pos.y;
        if (land.collider == nullptr)
        {
            land.collider = new Collider({ land.pos.x,land.pos.y,SCREEN_WIDTH,200 }, Collider::Type::SOLID, this);
            App->physics->AddObject(&land);
        }
    }
    else
    {
        water.mass = 0.0f;
        water.pos.x = 0;
        water.pos.y = 777;
        water.pastPos.x = water.pos.x;
        water.pastPos.y = water.pos.y;
        if (water.collider == nullptr)
        {
            water.collider = new Collider({ water.pos.x,water.pos.y,SCREEN_WIDTH,180 }, Collider::Type::WATER, this);
            App->physics->AddObject(&water);
        }
    }
}

void ModuleSceneIntro::DeleteEarth()
{
    if (land.collider != nullptr)
        App->physics->RemoveObject(&land);

    if (water.collider != nullptr)
        App->physics->RemoveObject(&water);
}

void ModuleSceneIntro::OnCollision(Collider* c1, Collider* c2)
{}