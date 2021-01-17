#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

#define MAX_COLLIDERS 250

class Collider
{
public:
	// Collision Type enum
	enum Type
	{
		NONE = -1,
		SPACE,
		SOLID,
		WATER,
		MOON,
		PLAYER,
		ASTEROID,
		MAX
	};

	// Constructor
	Collider(SDL_Rect rect, Type type, Module* listener = nullptr) : rect(rect), type(type), listener(listener)
	{}

	// Sets the position of the construct
	void SetPos(int x, int y, int w, int h);

	// Checks if two rects are intersecting
	bool Intersects(const SDL_Rect& r) const;

	SDL_Rect rect;
	bool pendingToDelete = false;
	Type type;
	Module* listener = nullptr;
};

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

	// Adds a new collider to the list
	Collider* AddCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);

public:
	fPoint gravity;
	bool debug;
private:
	// All existing colliders in the scene
	Collider* colliders[MAX_COLLIDERS] = { nullptr };

	// The collision matrix. Defines the interaction for two collider types
	// If set to false, collider 1 will ignore collider 2
	bool matrix[Collider::Type::MAX][Collider::Type::MAX];
};