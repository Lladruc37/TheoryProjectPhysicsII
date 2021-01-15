#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = false);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
    update_status Update(float dt);
	update_status PostUpdate();
	bool CleanUp();

    fPoint Force2Accel(fPoint force, int mass);
    void UpdatePhysics(iPoint& pos, fPoint& speed, fPoint acceleration, float dt);
    void ResolveCollisions(iPoint& pos, iPoint nextPos, fPoint& speed, fPoint nextSpeed, int width, int height); // Requires Raycast for the future

public:
	fPoint gravity;

private:
	bool debug;
};