#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");
    gravity.x = 0.0f;
    gravity.y = 60.0f;
	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModulePhysics::Update(float dt)
{
    return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	return true;
}

fPoint ModulePhysics::Force2Accel(fPoint force, int mass)
{
    fPoint a;
    a.x = force.x / mass;
    a.y = force.y / mass;
    return a;
}

void ModulePhysics::UpdatePhysics(iPoint& pos, fPoint& speed, fPoint a, float dt)
{
    // verlet
    pos.x = pos.x + speed.x * dt + (a.x * dt * dt * 0.5);
    speed.x = speed.x + (a.x * dt);
    pos.y = pos.y + speed.y * dt + ((gravity.y + a.y) * dt * dt * 0.5);
    speed.y = speed.y + ((gravity.y + a.y) * dt);

    if (speed.y > 200.0f)
        speed.y = 200.0f;
    else if (speed.y < -200.0f)
        speed.y = -200.0f;
    if (speed.x > 200.0f)
        speed.x = 200.0f;
    else if (speed.x < -200.0f)
        speed.x = -200.0f;
    //LOG("pos: %d, %d acc: %f, %f", pos.x, pos.y, a.x, a.y);
}

void ModulePhysics::ResolveCollisions(iPoint& pos, iPoint nextPos, fPoint& speed, fPoint nextSpeed, int width, int height)
{
    //if ((nextPos.x + width) > App->renderer->camera.x + App->renderer->camera.w)
    //{
    //    nextPos.x = App->renderer->camera.w - width;
    //}
    //if ((nextPos.x) < App->renderer->camera.x)
    //{
    //    nextPos.x = App->renderer->camera.w - width;
    //}




    LOG("pos: %d, %d speed: %f, %f", nextPos.x, nextPos.y, nextSpeed.x, nextSpeed.y);
    pos.x = nextPos.x;
    pos.y = nextPos.y;
    speed.x = nextSpeed.x;
    speed.y = nextSpeed.y;
}

