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

	SDL_Rect rect;
	bool pendingToDelete = false;
	Type type;
	Module* listener = nullptr;
};

struct Object
{
	enum Shape
	{
		RECT = 0,
		CIRCLE
	};

	Object(iPoint pos, fPoint speed, fPoint force, float mass = 0.0f, Shape shape = Shape::RECT, Collider* c = nullptr, float angle = 0.0f) : pos(pos), speed(speed), force(force), mass(mass), shape(shape), collider(c), angle(angle)
	{}

	Object()
	{}

	iPoint pos;
	fPoint speed;
	fPoint force;
	float mass;
	Shape shape;
	Collider* collider;
	float angle;
};

struct Circle : public Object
{
	int radius;
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

    void UpdatePhysics(Object* object, float dt);
    void ResolveCollisions(Object* object, iPoint pastPos, fPoint pastSpeed, int width, int height); // Requires Raycast for the future

	// Adds a new collider to the list
	//Collider* AddCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);
	void AddObject(Object* object);
	void RemoveObject(Object* object);

private:
	fPoint Force2Accel(fPoint force, int mass);
	void UpdateGravity();

	// Checks if two rects are intersecting
	//bool Intersects(const SDL_Rect& r) const;
	bool Intersects(Object* A, Object* B) const;

public:
	fPoint gravity;
	bool debug;

private:
	// All existing colliders in the scene
	//Collider* colliders[MAX_COLLIDERS] = { nullptr };
	p2List<Object*> objects;

	// The collision matrix. Defines the interaction for two collider types
	// If set to false, collider 1 will ignore collider 2
	bool matrix[Collider::Type::MAX][Collider::Type::MAX];
};