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

    Object player;
    //iPoint position;
    //iPoint nextPos;
    iPoint pastPos;
    int width;
    int height;
    float angle;
    //fPoint speed;
    //fPoint nextSpeed;
    fPoint pastSpeed;
    //fPoint totalForce;
    //int mass;
    bool isDestroyed;

    bool isMovingUp;
    bool isMovingLeft;
    bool isMovingRight;
    bool isMovingDown;
    int movingFx;
    int explosionFx;
    int movingChannelOne;
    int movingChannelTwo;
    int movingChannelThree;
    int movingChannelFour;
};