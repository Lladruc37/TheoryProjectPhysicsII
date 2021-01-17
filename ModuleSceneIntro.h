#pragma once
#include "Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "Globals.h"

class Collider;
struct Object;

enum Scene
{
    TITLE_SCREEN,
    GAME
};

enum GameScreen
{
    EARTH,
    ASTEROIDS,
    MOON
};

struct Asteroid : public Object
{
    //iPoint pos;
    int width;
    int height;
    float angle;
    //fPoint speed;
    //int mass;
    //Collider* collider = nullptr;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
    update_status PreUpdate();
	update_status Update(float dt);
    update_status PostUpdate();
	bool CleanUp();
    void OnCollision(Collider* c1, Collider* c2);

    void CreateAsteroids();

public:
    Scene currentScene = TITLE_SCREEN;
    GameScreen currentScreen = EARTH;

    // GAME //
    SDL_Texture* bgEarth = nullptr;
    SDL_Texture* bgAsteroids = nullptr;
    SDL_Texture* bgMoon = nullptr;
    bool startGame = false;

    SDL_Texture* asteroidTxt = nullptr;
    Asteroid top;
    Asteroid mid[3];
    Asteroid bot[4];

    // WIN/LOSE //
    SDL_Texture* gameOverTxt = nullptr;

    // TITLE SCREEN //
    SDL_Texture* backgroundTitle = nullptr;
};
