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
            if ((App->player->player.pos.y + App->player->player.radius / 2) <= 0)
            {
                App->player->player.pos.y = App->renderer->camera.h - App->player->player.radius / 2;
                switch (currentScreen)
                {
                case EARTH:
                {
                    currentScreen = ASTEROIDS;
                    DeleteEarth();
                    CreateAsteroids();
                    break;
                }
                case ASTEROIDS:
                {
                    currentScreen = MOON;
                    DeleteAsteroids();
                    CreateMoon();
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
                    DeleteAsteroids();
                    CreateEarth();
                    break;
                }
                case MOON:
                {
                    currentScreen = ASTEROIDS;
                    DeleteMoon();
                    CreateAsteroids();
                    break;
                }
                }
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
            if (currentScreen == EARTH)
            {
                //land.force.y = -App->physics->gravity.y;
                //water.force.y = -App->physics->gravity.y;
                //if ((land.pos.y + land.collider->rect.h) > (App->renderer->camera.h)) //Bottom bound
                //{
                //    land.pos.y = App->renderer->camera.h - land.collider->rect.h - 1;
                //}
            }
            else if (currentScreen == ASTEROIDS)
            {
                //fPoint a;
                //a.x = 0;
                //a.y = 0;
                //App->physics->UpdatePhysics(top.pos, top.speed, a, dt);
                //App->physics->UpdatePhysics(&top, dt);
                if (top.collider != nullptr)
                {
                    if (top.pos.x >= App->renderer->camera.w + top.collider->rect.w)
                        top.pos.x = 0 - top.collider->rect.w;
                    //top.collider->SetPos(top.pos.x, top.pos.y, top.collider->rect.w, top.collider->rect.h);
                }
                top.angle += 150.0f * dt;

                for (int i = 0; i != 3; ++i)
                {
                    //App->physics->UpdatePhysics(mid[i].pos, mid[i].speed, a, dt);
                    //App->physics->UpdatePhysics(&mid[i], dt);
                    if (mid[i].collider != nullptr)
                    {
                        if (mid[i].pos.x <= 0 - mid[i].collider->rect.w)
                            mid[i].pos.x = App->renderer->camera.w + mid[i].collider->rect.w;
                        //mid[i].collider->SetPos(mid[i].pos.x, mid[i].pos.y, mid[i].collider->rect.w, mid[i].collider->rect.h);
                    }
                    mid[i].angle -= 100.0f * dt;
                }

                for (int i = 0; i != 4; ++i)
                {
                    //LOG("SpeedAsteroidX%d = %f",i, bot[i].speed.x);               Theres a bug with a strange increase of speed when asteroids are tpd
                    //App->physics->UpdatePhysics(bot[i].pos, bot[i].speed, a, dt);
                    //App->physics->UpdatePhysics(&bot[i], dt);
                    if (bot[i].collider != nullptr)
                    {
                        if (bot[i].pos.x >= App->renderer->camera.w + bot[i].collider->rect.w)
                            bot[i].pos.x = 0 - bot[i].collider->rect.w;
                        //bot[i].collider->SetPos(bot[i].pos.x, bot[i].pos.y, bot[i].collider->rect.w, bot[i].collider->rect.h);
                    }
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
                CreateEarth();
                startGame = false;
            }
            App->renderer->Blit(backgroundTitle, 0, 0,true);
            break;
        }
        case GAME:
        {
            switch (currentScreen)
            {
                case EARTH:
                {
                    App->renderer->Blit(bgEarth, 0, 0, true);

                    if (App->physics->debug && land.collider != nullptr)
                    {
                        App->renderer->DrawQuad(land.collider->rect, 255, 0, 0, 100);
                    }
                    if (App->physics->debug && water.collider != nullptr)
                    {
                        App->renderer->DrawQuad(water.collider->rect, 0, 0, 255, 100);
                    }
                    break;
                }
                case ASTEROIDS:
                {
                    App->renderer->Blit(bgAsteroids, 0, 0, true);
                    App->renderer->Blit(asteroidTxt, top.pos.x - top.radius, top.pos.y - top.radius, false, top.angle);
                    for (int i = 0; i != 3; ++i)
                    App->renderer->Blit(asteroidTxt, mid[i].pos.x - mid[i].radius, mid[i].pos.y - mid[i].radius, false, mid[i].angle);
                    for (int i = 0; i != 4; ++i)
                    App->renderer->Blit(asteroidTxt, bot[i].pos.x - bot[i].radius, bot[i].pos.y - bot[i].radius, false, bot[i].angle);

                    if (App->physics->debug)
                    {
                        if (top.collider != nullptr)
                        App->renderer->DrawCircle(top.pos.x, top.pos.y,top.radius, 255, 0, 0, 100);
                        for (int i = 0; i != 3; ++i)
                        {
                            if(mid[i].collider != nullptr)
                            App->renderer->DrawCircle(mid[i].pos.x,mid[i].pos.y,mid[i].radius, 255, 0, 0, 100);
                        }
                        for (int i = 0; i != 4; ++i)
                        {
                            if(bot[i].collider != nullptr)
                            App->renderer->DrawCircle(bot[i].pos.x,bot[i].pos.y,bot[i].radius, 255, 0, 0, 100);
                        }
                    }
                    break;
                }
                case MOON:
                {
                    App->renderer->Blit(bgMoon, 0, 0, true);

                    if (App->physics->debug)
                    {
                        App->renderer->DrawCircle(moon.pos.x,moon.pos.y,moon.radius, 255, 0, 0, 100);
                    }
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
    //top.pos.x = -70;
    //top.pos.y = 135;
    //top.angle = (float)(rand() % 360);
    //top.speed.x = 550.0f;
    //top.mass = 3;
    //if (top.collider == nullptr)
    //{
    //    top.collider = new Collider({ top.pos.x,top.pos.y,70,70 }, Collider::Type::ASTEROID, this);
    //    App->physics->AddObject(&top);
    //}

    top.mass = 1.0f;
    top.radius = 35;
    top.pos.x = -70;
    top.pos.y = 135;
    top.angle = (float)(rand() % 360);
    top.speed.x = 550.0f;
    top.shape = Object::Shape::CIRCLE;
    if (top.collider == nullptr)
    {
        top.collider = new Collider({ top.pos.x - top.radius,top.pos.y - top.radius,top.radius * 2,top.radius * 2 }, Collider::Type::ASTEROID, this);
        App->physics->AddObject(&top);
    }

    for (int i = 0; i != 3; ++i)
    {
        //mid[i].pos.x = App->renderer->camera.w + 70 - (i * 350);
        //mid[i].pos.y = 375;
        //mid[i].angle = (float)(rand() % 360);
        //mid[i].speed.x = -300.0f;
        //mid[i].mass = 3;
        //if (mid[i].collider == nullptr)
        //{
        //    mid[i].collider = new Collider({ mid[i].pos.x,mid[i].pos.y,70,70 }, Collider::Type::ASTEROID, this);
        //    App->physics->AddObject(&mid[i]);
        //}

        mid[i].mass = 1.0f;
        mid[i].radius = 35;
        mid[i].pos.x = App->renderer->camera.w + 105 - (i * 350);//70
        mid[i].pos.y = 410;//375
        mid[i].angle = (float)(rand() % 360);
        mid[i].speed.x = -300.0f;
        mid[i].shape = Object::Shape::CIRCLE;
        if (mid[i].collider == nullptr)
        {
            mid[i].collider = new Collider({ mid[i].pos.x - mid[i].radius,mid[i].pos.y - mid[i].radius,mid[i].radius * 2,mid[i].radius * 2 }, Collider::Type::ASTEROID, this);
            App->physics->AddObject(&mid[i]);
        }
    }

    for (int i = 0; i != 4; ++i)
    {
        //bot[i].pos.x = 70 + (i * 262);
        //bot[i].pos.y = 615;
        //bot[i].angle = (float)(rand() % 360);
        //bot[i].speed.x = 165.0f;
        //bot[i].mass = 3;
        //if (bot[i].collider == nullptr)
        //{
        //    bot[i].collider = new Collider({ bot[i].pos.x,bot[i].pos.y,70,70 }, Collider::Type::ASTEROID, this);
        //    App->physics->AddObject(&bot[i]);
        //}


        bot[i].mass = 1.0f;
        bot[i].radius = 35;
        bot[i].pos.x = 105 + (i * 262);//70
        bot[i].pos.y = 650;//615
        bot[i].angle = (float)(rand() % 360);
        bot[i].speed.x = 165.0f;
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
    {
        App->physics->RemoveObject(&top);
        top.collider = nullptr;
    }
    for (int i = 0; i != 3; ++i)
    {
        if (mid[i].collider != nullptr)
        {
            App->physics->RemoveObject(&mid[i]);
            mid[i].collider = nullptr;
        }
    }
    for (int i = 0; i != 4; ++i)
    {
        if (bot[i].collider != nullptr)
        {
            App->physics->RemoveObject(&bot[i]);
            bot[i].collider = nullptr;
        }
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
    {
        App->physics->RemoveObject(&moon);
        moon.collider = nullptr;
    }
}

void ModuleSceneIntro::CreateEarth()
{
    land.pos.x = 0;
    land.pos.y = 740;
    land.pastPos.x = land.pos.x;
    land.pastPos.y = land.pos.y;
    land.mass = 0.0f;
    if (land.collider == nullptr)
    {
        land.collider = new Collider({ land.pos.x,land.pos.y,570,180 }, Collider::Type::SOLID, this);
        App->physics->AddObject(&land);
    }

    water.pos.x = 570;
    water.pos.y = 800;
    water.mass = 0.0f;
    if (water.collider == nullptr)
    {
        water.collider = new Collider({ water.pos.x,water.pos.y,350,180 }, Collider::Type::WATER, this);
        App->physics->AddObject(&water);
    }
}

void ModuleSceneIntro::DeleteEarth()
{
    if (land.collider != nullptr)
    {
        App->physics->RemoveObject(&land);
        land.collider = nullptr;
    }
    if (water.collider != nullptr)
    {
        App->physics->RemoveObject(&water);
        water.collider = nullptr;
    }
}

void ModuleSceneIntro::OnCollision(Collider* c1, Collider* c2) {}