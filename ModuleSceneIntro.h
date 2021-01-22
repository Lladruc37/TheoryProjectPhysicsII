#ifndef __MODULE_SCENE_INTRO_H__
#define __MODULE_SCENE_INTRO_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2DynArray.h"
#include "Object.h"

struct SDL_Texture;

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

class ModuleSceneIntro : public Module
{
public:
    ModuleSceneIntro(Application* app, bool startEnabled = true);
    ~ModuleSceneIntro();

    bool Start();
    UpdateStatus PreUpdate();
    UpdateStatus Update(float dt);
    UpdateStatus PostUpdate();
    bool CleanUp();
    void OnCollision(Collider* c1, Collider* c2);

    // Create/Delete objects from scene
    void CreateAsteroids();
    void DeleteAsteroids();
    void CreateMoon();
    void DeleteMoon();
    void CreateEarth(bool checkpoint);
    void DeleteEarth();

public:
    Scene currentScene = TITLE_SCREEN;
    GameScreen currentScreen = EARTH;

    // GAME //
    bool startGame = false;

    Circle top;
    Circle mid[3];
    Circle bot[4];
    Circle moon;
    Object land;
    Object water;

    SDL_Texture* bgEarth = nullptr;
    SDL_Texture* bgWater = nullptr;
    SDL_Texture* bgAsteroids = nullptr;
    SDL_Texture* bgMoon = nullptr;
    SDL_Texture* flagTxt = nullptr;
    SDL_Texture* asteroidTxt = nullptr;

    // WIN/LOSE //
    SDL_Texture* gameOverTxt = nullptr;
    SDL_Texture* victoryTxt = nullptr;

    // TITLE SCREEN //
    SDL_Texture* bgTitle = nullptr;
};

#endif // !__MODULE_SCENE_INTRO_H__