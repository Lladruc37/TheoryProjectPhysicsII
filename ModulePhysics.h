#ifndef __MODULE_PHYSICS_H__
#define __MODULE_PHYSICS_H__

#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "Object.h"

#define WATER_DENSITY 0.0025
#define DAMPEN 0.975

class Collider
{
public:
	// Collision Type enum
	enum Type
	{
		NONE = -1,
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

	SDL_Rect rect;
	Type type;
	Module* listener = nullptr;
};

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool startEnabled = false);
	~ModulePhysics();

	bool Start();
	UpdateStatus PreUpdate();
	UpdateStatus Update(float dt);
	bool CleanUp();

	// Adds a new object to the list
	void AddObject(Object* object);
	// Removes an existing object from the list
	void RemoveObject(Object* object);

	void UpdateDrag(fPoint direction);

private:
	// Calculates acceleration from force
	fPoint Force2Accel(fPoint force, int mass);

	// Updates gravity
	void UpdateGravity();

	// Steps an object in the world
	void UpdatePhysics(Object* object, float dt);

	// Checks if two objects are intersecting
	bool Intersects(Object* A, Object* B);

	// Resolves two objects colliding
	void ResolveCollisions(Object* A, Object* B);

	// Calculates the shortest distance between a circle and a rect
	float ShortestDist(Object* A, Circle* B, iPoint& rPos);

	// Applies buoyancy to the object B
	void Buoyancy(Object* A, Object* B);

public:
	fPoint gravity;
	bool debug;
	fPoint dragDirection;

private:
	// List of all existing objects in the scene
	p2List<Object*> objects;

	// The collision matrix
	// Defines the interaction for two collider types
	// If set to false, collider 1 will ignore collider 2
	bool matrix[Collider::Type::MAX][Collider::Type::MAX];
};

#endif // !__MODULE_PHYSICS_H__