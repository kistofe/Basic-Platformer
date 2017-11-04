#include "j1App.h"
#include "p2Log.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1SceneSwitch.h"
#include "j1Scene.h"

j1Collision::j1Collision()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;


	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_ENDOFLEVEL] = false;
	matrix[COLLIDER_WALL][COLLIDER_FPLAYER] = true;

	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_ENDOFLEVEL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_FPLAYER] = false;

	matrix[COLLIDER_ENDOFLEVEL][COLLIDER_WALL] = true;
	matrix[COLLIDER_ENDOFLEVEL][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_ENDOFLEVEL][COLLIDER_ENDOFLEVEL] = true;
	matrix[COLLIDER_ENDOFLEVEL][COLLIDER_FPLAYER] = true;

	matrix[COLLIDER_FPLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_FPLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_FPLAYER][COLLIDER_ENDOFLEVEL] = true;
	matrix[COLLIDER_FPLAYER][COLLIDER_FPLAYER] = false;
}

j1Collision::~j1Collision()
{
}

bool j1Collision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	// Check collision between colliders, and call OnCollision if two are colliding
	Collider* c1;
	Collider* c2;

	//Create a rectangle to store intersecion data
	SDL_Rect intersection;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];
		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			//check if two colliders' rectangles are intersecting
			if (SDL_IntersectRect(&c1->rect, &c2->rect, &intersection))
			{
				//if the collision between the two collider types is allowed...
				if (matrix[c1->type][c2->type])
					OnCollision(c1, c2, &intersection); //...call method OnCollision
			}

		}
	}

	return true;
}

bool j1Collision::Update(float dt)
{
	DebugDraw();
	
	return true;
}

bool j1Collision::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type);
			break;
		}
	}

	LOG("Collider created successfully. Attributes: x=%d, y=%d, w=%d; h=%d", ret->rect.x, ret->rect.y, ret->rect.w, ret->rect.h);

	return ret;
}

bool j1Collision::EraseCollider(Collider * collider)
{
	collider->to_delete = true;
	return true;
}

void j1Collision::DebugDraw()
{
	if ((App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN))
		debug = !debug;

	if (debug == false)
		return;

	uint alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;
		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, 0);
			break;
		case COLLIDER_WALL: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_ENDOFLEVEL: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_FPLAYER:
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
		}
	}
}

bool j1Collision::OnCollision(Collider* c1, Collider* c2, SDL_Rect* intersection)
{
	bool ret = false;

	// If player's future collider detected a collision with a wall, prevent the player's collision
	if (c1->type == COLLIDER_FPLAYER && c2->type == COLLIDER_WALL)
	{
		//if (c1->rect.x + c1->rect.w > c2->rect.x &&)
		//App->player->player_speed.x -= intersection->w;
	}

	return ret;
}

bool j1Collision::PostUpdate()
{

		return true;
}