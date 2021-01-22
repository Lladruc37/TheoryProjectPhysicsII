#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"

class Collider;
struct Object;

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = false);
	virtual ~ModulePlayer();

    void CreatePlayer();
    void DeletePlayer();
	bool Start();
    update_status PreUpdate();
	update_status Update(float dt);
    update_status PostUpdate();
	bool CleanUp();
    void OnCollision(Collider* c1, Collider* c2);

public:
    SDL_Texture* playerTex;
    SDL_Texture* explosion;
    Animation explosionAnim;
    float dtAnim;
    //Collider* collider = nullptr;

    Circle player;
    //iPoint position;
    //iPoint nextPos;
    //iPoint pastPos;
    //int width;
    //int height;
    //float angle;
    //fPoint speed;
    //fPoint nextSpeed;
    //fPoint pastSpeed;
    //fPoint totalForce;
    //int mass;
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