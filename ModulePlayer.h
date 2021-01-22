#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "Object.h"

class ModulePlayer : public Module
{
public:
    ModulePlayer(Application* app, bool startEnabled = false);
    virtual ~ModulePlayer();

    void CreatePlayer();
    void DeletePlayer();
    bool Start();
    UpdateStatus PreUpdate();
    UpdateStatus Update(float dt);
    UpdateStatus PostUpdate();
    bool CleanUp();
    void OnCollision(Collider* c1, Collider* c2);

public:
    SDL_Texture* playerTex;
    SDL_Texture* explosion;
    Animation explosionAnim;
    float dtAnim;

    Circle player;

    bool isDestroyed;
    bool godMode;
    bool checkpoint;
    bool victory;
    bool onceMusic;

    float flagAngle;
    iPoint flagPosition;
    uint flagFx;

    bool isMovingUp;
    bool isMovingLeft;
    bool isMovingRight;
    bool isMovingDown;
    uint movingFx;
    uint explosionFx;
    uint movingChannelOne;
    uint movingChannelTwo;
    uint movingChannelThree;
    uint movingChannelFour;
};

#endif // !__MODULE_PLAYER_H__