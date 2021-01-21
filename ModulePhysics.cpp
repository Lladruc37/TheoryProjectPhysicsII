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

            if (Intersects(tmp->data,tmp2->data) && matrix[c1->type][c2->type] && App->player->godMode == false)
            {
                ResolveCollisions(tmp->data, tmp2->data);
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
    {
        gravity.x = 0.0f;
        gravity.y = 200.0f;
        break;
    }
    case GameScreen::ASTEROIDS:
    {
        gravity.x = 0.0f;
        gravity.y = 0.0f;
        break;
    }
    case GameScreen::MOON:
    {
        Object* player = nullptr;
        Circle* moon = nullptr;
        p2List_item<Object*>* tmp = objects.getFirst();
        while (tmp != nullptr)
        {
            if (tmp->data->collider->type == Collider::Type::PLAYER)
            {
                player = tmp->data;
            }
            else if (tmp->data->collider->type == Collider::Type::MOON)
            {
                moon = (Circle*)tmp->data;
            }
            tmp = tmp->next;
        }
        //iPoint dif;
        //dif.x = player->pos.x - moon->pos.x;
        //dif.y = player->pos.y - moon->pos.y;


        //float dist = sqrtf((float)abs(dif.x) * (float)abs(dif.x) + (float)abs(dif.y) * (float)abs(dif.y));
        //fPoint ratio;
        //ratio.x = dif.x / dist;
        //ratio.y = dif.y / dist;



        iPoint rPos;
        // find closest X
        if (player->collider->rect.x > moon->pos.x)
        {
            // left
            rPos.x = player->collider->rect.x;
        }
        else if (player->collider->rect.x + player->collider->rect.w < moon->pos.x)
        {
            // right
            rPos.x = player->collider->rect.x + player->collider->rect.w;
        }
        else
        {
            // inside
            rPos.x = moon->pos.x;
        }

        // find closest Y
        if (player->collider->rect.y > moon->pos.y)
        {
            // top
            rPos.y = player->collider->rect.y;
        }
        else if (player->collider->rect.y + player->collider->rect.h < moon->pos.y)
        {
            // bot
            rPos.y = player->collider->rect.y + player->collider->rect.h;
        }
        else
        {
            // inside
            rPos.y = moon->pos.y;
        }
        //LOG("A: %d,%d B: %d,%d rPos: %d,%d", player->collider->rect.x, player->collider->rect.y, moon->pos.x, moon->pos.y, rPos.x, rPos.y);
        // compute distance
        float dist = CalculateModule(moon->pos, rPos);
        LOG("dist: %f", dist);
        LOG("ovni: %d,%d moon:%d,%d", rPos.x, rPos.y, moon->pos.x, moon->pos.y);


        fPoint dif;
        dif.x = rPos.x - moon->pos.x;
        dif.y = rPos.y - moon->pos.y;
        float otherDist = 500 - dist;
        if (otherDist <= 0.0f)
        {
            otherDist = 0.0f;
        }

        //gravity.x = 500000 * player->mass / (dif.x * dif.x);
        //gravity.y = 500000 * player->mass / (dif.y * dif.y);

        if (dif.x > 0.0f && dif.y > 0.0f) //top left
        {
            gravity.x = 5000000 * player->mass / -(dif.x * dif.x);
            gravity.y = 5000000 * player->mass / -(dif.y * dif.y);
            //gravity.x = -otherDist * cos(dif.x / dist);
            //gravity.y = -otherDist * sin(dif.y / dist);
        }
        else if (dif.x > 0.0f && dif.y < 0.0f) // top right
        {
            gravity.x = 5000000 * player->mass / -(dif.x * dif.x);
            gravity.y = 5000000 * player->mass / (dif.y * dif.y);
            //gravity.x = -otherDist * cos(dif.x / dist);
            //gravity.y = otherDist * sin(-dif.y / dist);
        }
        else if (dif.x < 0.0f && dif.y > 0.0f) //bot left
        {
            gravity.x = 5000000 * player->mass / (dif.x * dif.x);
            gravity.y = 5000000 * player->mass / -(dif.y * dif.y);
            //gravity.x = otherDist * cos(dif.x / dist);
            //gravity.y = -otherDist * sin(dif.y / dist);
        }
        else if (dif.x < 0.0f && dif.y < 0.0f) //bot right
        {
            gravity.x = 5000000 * player->mass / (dif.x * dif.x);
            gravity.y = 5000000 * player->mass / (dif.y * dif.y);
            //gravity.x = otherDist * cos(dif.x / dist);
            //gravity.y = otherDist * sin(-dif.y / dist);
        }
        //gravity.x = gravity.x / player->mass;
        //gravity.y = gravity.y / player->mass;
        LOG("Gravity: %f,%f", gravity.x, gravity.y);

        if (gravity.x > 180.0f)
        {
            gravity.x = 180.0f;
        }
        else if (gravity.x < -180.0f)
        {
            gravity.x = -180.0f;
        }
        if (gravity.y > 180.0f)
        {
            gravity.y = 180.0f;
        }
        else if (gravity.y < -180.0f)
        {
            gravity.y = -180.0f;
        }
        LOG("Gravity: %f,%f", gravity.x, gravity.y);
        //gravity.x = 0.0f;
        //gravity.y = 0.0f;
        break;
    }
    default:
    {
        break;
    }
    }
}

void ModulePhysics::UpdatePhysics(Object* object, float dt)
{
    // update past variables
    object->pastPos = object->pos;
    object->pastSpeed = object->speed;

    fPoint a = Force2Accel(object->force, object->mass);
    // verlet
    object->pos.x = object->pos.x + object->speed.x * dt + ((gravity.x + a.x) * dt * dt * 0.5);
    object->speed.x = object->speed.x + ((gravity.x + a.x) * dt);
    object->pos.y = object->pos.y + object->speed.y * dt + ((gravity.y + a.y) * dt * dt * 0.5);
    object->speed.y = object->speed.y + ((gravity.y + a.y) * dt);

    // reset force
    object->force.x = 0.0f;
    object->force.y = 0.0f;

    // update the collider position
    if (object->collider != nullptr)
    {
        if (object->shape == Object::Shape::CIRCLE)
        {
            Circle* tmp = (Circle*)object;
            object->collider->SetPos(object->pos.x - tmp->radius, object->pos.y - tmp->radius, object->collider->rect.w, object->collider->rect.h);
        }
        else
        {
            object->collider->SetPos(object->pos.x, object->pos.y, object->collider->rect.w, object->collider->rect.h);
        }
    }
    //LOG("pos: %d, %d acc: %f, %f", pos.x, pos.y, a.x, a.y);
}

void ModulePhysics::ResolveCollisions(Object* A, Object* B) 
{
    if (A->collider->type == Collider::Type::WATER || B->collider->type == Collider::Type::WATER)
    {
        //BUOYANCY
        if (A->collider->type == Collider::Type::WATER)
        {
            ResetSpeed(A);
            Buoyancy(A, B);
        }
        else
        {
            ResolveCollisions(B, A);
        }
    }
    else
    {
        ResetSpeed(A);
        ResetSpeed(B);
        if (A->shape == Object::Shape::RECT && B->shape == Object::Shape::RECT)
        {
            fPoint frameDifB;
            frameDifB.x = B->pos.x - B->pastPos.x;
            frameDifB.y = B->pos.y - B->pastPos.y;
            iPoint tmpB;
            tmpB.x = B->pos.x;
            tmpB.y = B->pos.y;
            fPoint frameDifA;
            frameDifA.x = A->pos.x - A->pastPos.x;
            frameDifA.y = A->pos.y - A->pastPos.y;
            iPoint tmpA;
            tmpA.x = A->pos.x;
            tmpA.y = A->pos.y;
            while (Intersects(A, B))
            {
                if (frameDifB.x > 0)
                {
                    tmpB.x--;
                    frameDifB.x--;
                }
                else if (frameDifB.x < 0)
                {
                    tmpB.x++;
                    frameDifB.x++;
                }
                if (frameDifB.y > 0)
                {
                    tmpB.y--;
                    frameDifB.y--;
                }
                else if (frameDifB.y < 0)
                {
                    tmpB.y++;
                    frameDifB.y++;
                }

                if (frameDifA.x > 0)
                {
                    tmpA.x--;
                    frameDifA.x--;
                }
                else if (frameDifA.x < 0)
                {
                    tmpA.x++;
                    frameDifA.x++;
                }
                if (frameDifA.y > 0)
                {
                    tmpA.y--;
                    frameDifA.y--;
                }
                else if (frameDifA.y < 0)
                {
                    tmpA.y++;
                    frameDifA.y++;
                }

                LOG("%d,%d - %d,%d", tmpB.x, tmpB.y, tmpA.x, tmpA.y);
                A->collider->rect.x = tmpA.x;
                A->collider->rect.y = tmpA.y;
                B->collider->rect.x = tmpB.x;
                B->collider->rect.y = tmpB.y;
            }
            B->pos.x = tmpB.x;
            B->pos.y = tmpB.y;
            A->pos.x = tmpA.x;
            A->pos.y = tmpA.y;
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

            // compute distance
            float dist = CalculateModule(rPos, B->pos);

            fPoint frameDifB;
            frameDifB.x = B->pos.x - B->pastPos.x;
            frameDifB.y = B->pos.y - B->pastPos.y;
            iPoint tmpB;
            tmpB.x = B->pos.x;
            tmpB.y = B->pos.y;
            fPoint frameDifA;
            frameDifA.x = A->pos.x - A->pastPos.x;
            frameDifA.y = A->pos.y - A->pastPos.y;
            iPoint tmpA;
            tmpA.x = rPos.x;
            tmpA.y = rPos.y;
            while (dist < circle->radius)
            {
                if (frameDifB.x > 0)
                {
                    tmpB.x--;
                    frameDifB.x--;
                }
                else if (frameDifB.x < 0)
                {
                    tmpB.x++;
                    frameDifB.x++;
                }
                if (frameDifB.y > 0)
                {
                    tmpB.y--;
                    frameDifB.y--;
                }
                else if (frameDifB.y < 0)
                {
                    tmpB.y++;
                    frameDifB.y++;
                }

                if (frameDifA.x > 0)
                {
                    tmpA.x--;
                    frameDifA.x--;
                }
                else if (frameDifA.x < 0)
                {
                    tmpA.x++;
                    frameDifA.x++;
                }
                if (frameDifA.y > 0)
                {
                    tmpA.y--;
                    frameDifA.y--;
                }
                else if (frameDifA.y < 0)
                {
                    tmpA.y++;
                    frameDifA.y++;
                }
                dist = CalculateModule(tmpA, tmpB);
                LOG("%d,%d - %d,%d = %f", tmpB.x, tmpB.y, tmpA.x, tmpA.y, dist);
            }
            B->pos.x = tmpB.x;
            B->pos.y = tmpB.y;
            A->pos.x += tmpA.x - rPos.x;
            A->pos.y += tmpA.y - rPos.y;
        }
        else if (A->shape == Object::Shape::CIRCLE && B->shape == Object::Shape::RECT)
        {
            ResolveCollisions(B, A);
        }
        else if (A->shape == Object::Shape::CIRCLE && B->shape == Object::Shape::CIRCLE)
        {
            Circle* circleA = (Circle*)A;
            Circle* circleB = (Circle*)B;
            float dist = CalculateModule(A->pos, B->pos);

            fPoint frameDifB;
            frameDifB.x = B->pos.x - B->pastPos.x;
            frameDifB.y = B->pos.y - B->pastPos.y;
            iPoint tmpB;
            tmpB.x = B->pos.x;
            tmpB.y = B->pos.y;
            fPoint frameDifA;
            frameDifA.x = A->pos.x - A->pastPos.x;
            frameDifA.y = A->pos.y - A->pastPos.y;
            iPoint tmpA;
            tmpA.x = A->pos.x;
            tmpA.y = A->pos.y;
            while (dist < (circleA->radius + circleB->radius))
            {
                if (frameDifB.x > 0)
                {
                    tmpB.x--;
                    frameDifB.x--;
                }
                else if (frameDifB.x < 0)
                {
                    tmpB.x++;
                    frameDifB.x++;
                }
                if (frameDifB.y > 0)
                {
                    tmpB.y--;
                    frameDifB.y--;
                }
                else if (frameDifB.y < 0)
                {
                    tmpB.y++;
                    frameDifB.y++;
                }

                if (frameDifA.x > 0)
                {
                    tmpA.x--;
                    frameDifA.x--;
                }
                else if (frameDifA.x < 0)
                {
                    tmpA.x++;
                    frameDifA.x++;
                }
                if (frameDifA.y > 0)
                {
                    tmpA.y--;
                    frameDifA.y--;
                }
                else if (frameDifA.y < 0)
                {
                    tmpA.y++;
                    frameDifA.y++;
                }
                dist = CalculateModule(tmpA, tmpB);
            }
            B->pos.x = tmpB.x;
            B->pos.y = tmpB.y;
            A->pos.x = tmpA.x;
            A->pos.y = tmpA.y;
        }
    }
}

void ModulePhysics::ResetSpeed(Object* A)
{
    A->speed.x = 0.0f;
    A->speed.y = 0.0f;
}

void ModulePhysics::AddObject(Object* object)
{
    objects.add(object);
}

void ModulePhysics::RemoveObject(Object* object)
{
    p2List_item<Object*>* tmp = objects.findNode(object);
    objects.del(tmp);
}

float ModulePhysics::CalculateModule(iPoint A, iPoint B)
{
    float dist = sqrtf((float)abs(B.x - A.x) * (float)abs(B.x - A.x) + (float)abs(B.y - A.y) * (float)abs(B.y - A.y));
    return dist;
}

float ModulePhysics::ShortestDist(Object* A, Circle* B)
{
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
    return CalculateModule(rPos, B->pos);
}

void Collider::SetPos(int x, int y, int w, int h)
{
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

bool ModulePhysics::Intersects(Object* A, Object* B)
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
        float dist = ShortestDist(A, circle);
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
        float dist = CalculateModule(A->pos, B->pos);
        LOG("dist: %f", dist);
        return (dist < (circleA->radius + circleB->radius));
    }
}

void ModulePhysics::Buoyancy(Object* A, Object* B)
{
    float volume;
    if (A->collider->rect.y <= B->collider->rect.y)
    {
        volume = B->collider->rect.h * B->collider->rect.w;
    }
    else
    {
        volume = (float)abs((A->collider->rect.y - B->collider->rect.y) - B->collider->rect.h) * B->collider->rect.w;
    }
    float tmpForce = B->force.y + (B->mass * gravity.y - (WATER_DENSITY)*volume * gravity.y);
    B->speed.y *= DAMPEN;
    //if (tmpForce < -600.0f)
    //{
    //    tmpForce = -600.0f;
    //}
    B->force.y = tmpForce;
    LOG("Force: %f, volum: %f", B->force.y, volume);
}