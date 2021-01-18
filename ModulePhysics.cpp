#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;

    matrix[Collider::Type::SOLID][Collider::Type::SOLID] = false;
    matrix[Collider::Type::SOLID][Collider::Type::WATER] = false;
    matrix[Collider::Type::SOLID][Collider::Type::MOON] = false;
    matrix[Collider::Type::SOLID][Collider::Type::PLAYER] = true;
    matrix[Collider::Type::SOLID][Collider::Type::ASTEROID] = false;

    matrix[Collider::Type::WATER][Collider::Type::SOLID] = false;
    matrix[Collider::Type::WATER][Collider::Type::WATER] = false;
    matrix[Collider::Type::WATER][Collider::Type::MOON] = false;
    matrix[Collider::Type::WATER][Collider::Type::PLAYER] = true;
    matrix[Collider::Type::WATER][Collider::Type::ASTEROID] = false;

    matrix[Collider::Type::MOON][Collider::Type::SOLID] = false;
    matrix[Collider::Type::MOON][Collider::Type::WATER] = false;
    matrix[Collider::Type::MOON][Collider::Type::MOON] = false;
    matrix[Collider::Type::MOON][Collider::Type::PLAYER] = true;
    matrix[Collider::Type::MOON][Collider::Type::ASTEROID] = false;

    matrix[Collider::Type::PLAYER][Collider::Type::SOLID] = true;
    matrix[Collider::Type::PLAYER][Collider::Type::WATER] = true;
    matrix[Collider::Type::PLAYER][Collider::Type::MOON] = true;
    matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
    matrix[Collider::Type::PLAYER][Collider::Type::ASTEROID] = true;

    matrix[Collider::Type::ASTEROID][Collider::Type::SOLID] = false;
    matrix[Collider::Type::ASTEROID][Collider::Type::WATER] = false;
    matrix[Collider::Type::ASTEROID][Collider::Type::MOON] = false;
    matrix[Collider::Type::ASTEROID][Collider::Type::PLAYER] = true;
    matrix[Collider::Type::ASTEROID][Collider::Type::ASTEROID] = false;
}

// Destructor
ModulePhysics::~ModulePhysics()
{}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");
    gravity.x = 0.0f;
    gravity.y = 0.0f;
	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
    // Delete colliders scheduled for deletion
    p2List_item<Object*>* tmp = objects.getFirst();
    while (tmp != nullptr)
    {
        if (tmp->data->collider != nullptr && tmp->data->collider->pendingToDelete == true)
        {
            delete tmp->data->collider;
            tmp->data->collider = nullptr;
        }

        tmp = tmp->next;
    }


    Collider* c1;
    Collider* c2;

    // Check collisions
    tmp = objects.getFirst();
    while(tmp != nullptr)
    {
        // skip empty colliders
        if (tmp->data->collider == nullptr)
        {
            tmp = tmp->next;
            continue;
        }

        c1 = tmp->data->collider;

        // avoid checking collisions already checked
        p2List_item<Object*>* tmp2 = tmp->next;
        while(tmp2 != nullptr)
        {
            // skip empty colliders
            if (tmp2->data->collider == nullptr)
            {
                tmp2 = tmp2->next;
                continue;
            }

            c2 = tmp2->data->collider;

            if (Intersects(tmp->data,tmp2->data) && matrix[c1->type][c2->type])
            {
                // Resolve Collisions
                if (c1->listener)
                {
                    c1->listener->OnCollision(c1, c2);
                }
                if (c2->listener)
                {
                    c2->listener->OnCollision(c2, c1);
                }
            }
            tmp2 = tmp2->next;
        }
        tmp = tmp->next;
    }

	return UPDATE_CONTINUE;
}

update_status ModulePhysics::Update(float dt)
{
    UpdateGravity();

    p2List_item<Object*>* tmp = objects.getFirst();
    while (tmp != nullptr)
    {
        if (tmp->data->mass != 0.0f)
        {
            UpdatePhysics(tmp->data, dt);
        }
        tmp = tmp->next;
    }
    return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
    LOG("Freeing all colliders");

    objects.clear();

	return true;
}

fPoint ModulePhysics::Force2Accel(fPoint force, int mass)
{
    fPoint a;
    a.x = force.x / mass;
    a.y = force.y / mass;
    return a;
}

void ModulePhysics::UpdateGravity()
{
    switch (App->scene_intro->currentScreen)
    {
    case GameScreen::EARTH:
        gravity.x = 0.0f;
        gravity.y = 60.0f;
        break;
    case GameScreen::ASTEROIDS:
        gravity.x = 0.0f;
        gravity.y = 0.0f;
        break;
    case GameScreen::MOON:
        gravity.x = 0.0f;
        gravity.y = 0.0f;
        break;
    default:
        break;
    }
}

void ModulePhysics::UpdatePhysics(Object* object, float dt)
{
    fPoint a = Force2Accel(object->force, object->mass);
    // verlet
    object->pos.x = object->pos.x + object->speed.x * dt + (a.x * dt * dt * 0.5);
    object->speed.x = object->speed.x + (a.x * dt);
    object->pos.y = object->pos.y + object->speed.y * dt + ((gravity.y + a.y) * dt * dt * 0.5);
    object->speed.y = object->speed.y + ((gravity.y + a.y) * dt);

    object->force.x = 0.0f;
    object->force.y = 0.0f;
    //LOG("pos: %d, %d acc: %f, %f", pos.x, pos.y, a.x, a.y);
}

void ModulePhysics::ResolveCollisions(Object* object, iPoint pastPos, fPoint pastSpeed, int width, int height)
{
    //if ((nextPos.x + width) > App->renderer->camera.x + App->renderer->camera.w)
    //{
    //    nextPos.x = App->renderer->camera.w - width;
    //}
    //if ((nextPos.x) < App->renderer->camera.x)
    //{
    //    nextPos.x = App->renderer->camera.w - width;
    //}




    LOG("pos: %d, %d speed: %f, %f", object->pos.x, object->pos.y, object->speed.x, object->speed.y);
}

//Collider* ModulePhysics::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
//{
//    Collider* ret = nullptr;
//    // Adds the collider in the first null spot on the array
//    for (uint i = 0; i < MAX_COLLIDERS; ++i)
//    {
//        if (colliders[i] == nullptr)
//        {
//            ret = colliders[i] = new Collider(rect, type, listener);
//            break;
//        }
//    }
//
//    return ret;
//}

void ModulePhysics::AddObject(Object* object)
{
    objects.add(object);
}

void ModulePhysics::RemoveObject(Object* object)
{
    p2List_item<Object*>* tmp = objects.findNode(object);
    objects.del(tmp);
}

void Collider::SetPos(int x, int y, int w, int h)
{
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

bool ModulePhysics::Intersects(Object* A, Object* B) const
{
    if (A->shape == Object::Shape::RECT && B->shape == Object::Shape::RECT)
    {
        return (A->collider->rect.x < B->collider->rect.x + B->collider->rect.w
            && A->collider->rect.x + A->collider->rect.w > B->collider->rect.x
            && A->collider->rect.y < B->collider->rect.y + B->collider->rect.h
            && A->collider->rect.h + A->collider->rect.y > B->collider->rect.y);
    }
    else if (A->shape == Object::Shape::RECT && B->shape == Object::Shape::CIRCLE)
    {
        Circle* circle = (Circle*)B;
        iPoint rPos;
        // find closest X
        if (A->collider->rect.x > B->pos.x)
        {
            // left
            rPos.x = A->collider->rect.x;
        }
        else if (A->collider->rect.x + A->collider->rect.w < B->pos.x)
        {
            // right
            rPos.x = A->collider->rect.x + A->collider->rect.w;
        }
        else
        {
            // inside
            rPos.x = B->pos.x;
        }

        // find closest Y
        if (A->collider->rect.y > B->pos.y)
        {
            // top
            rPos.y = A->collider->rect.y;
        }
        else if (A->collider->rect.y + A->collider->rect.h < B->pos.y)
        {
            // bot
            rPos.y = A->collider->rect.y + A->collider->rect.h;
        }
        else
        {
            // inside
            rPos.y = B->pos.y;
        }
        LOG("A: %d,%d B: %d,%d rPos: %d,%d", A->collider->rect.x, A->collider->rect.y, B->pos.x, B->pos.y, rPos.x, rPos.y);
        // compute distance
        float dist = sqrtf((float)abs(B->pos.x - rPos.x) * (float)abs(B->pos.x - rPos.x) + (float)abs(B->pos.y - rPos.y) * (float)abs(B->pos.y - rPos.y));
        LOG("dist: %f", dist);
        return (dist < circle->radius);
    }
    else if (A->shape == Object::Shape::CIRCLE && B->shape == Object::Shape::RECT)
    {
        return Intersects(B, A);
    }
    else if (A->shape == Object::Shape::CIRCLE && B->shape == Object::Shape::CIRCLE)
    {
        Circle* circleA = (Circle*)A;
        Circle* circleB = (Circle*)B;
        float dist = sqrtf(abs(B->collider->rect.x - A->collider->rect.x) * abs(B->collider->rect.x - A->collider->rect.x) + abs(B->collider->rect.y - A->collider->rect.y) * abs(B->collider->rect.y - A->collider->rect.y));
        return (dist > (circleA->radius + circleB->radius));
    }
}