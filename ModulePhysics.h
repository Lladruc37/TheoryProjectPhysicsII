#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
    update_status Update(float dt);
	update_status PostUpdate();
	bool CleanUp();

    void UpdatePhysics(iPoint& pos, fPoint& speed, fPoint acceleration, float dt);

private:
    float gravity = 600.0f;
	bool debug;
};