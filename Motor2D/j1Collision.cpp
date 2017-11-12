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
	matrix[COLLIDER_WALL][COLLIDER_DEATH] = false;
	matrix[COLLIDER_WALL][COLLIDER_ENEMY] = true;

	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_ENDOFLEVEL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_FPLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_DEATH] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;

	matrix[COLLIDER_ENDOFLEVEL][COLLIDER_WALL] = true;
	matrix[COLLIDER_ENDOFLEVEL][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_ENDOFLEVEL][COLLIDER_ENDOFLEVEL] = true;
	matrix[COLLIDER_ENDOFLEVEL][COLLIDER_FPLAYER] = true;
	matrix[COLLIDER_ENDOFLEVEL][COLLIDER_DEATH] = false;
	matrix[COLLIDER_ENDOFLEVEL][COLLIDER_ENEMY] = false;

	matrix[COLLIDER_FPLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_FPLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_FPLAYER][COLLIDER_ENDOFLEVEL] = true;
	matrix[COLLIDER_FPLAYER][COLLIDER_FPLAYER] = false;
	matrix[COLLIDER_FPLAYER][COLLIDER_DEATH] = false;
	matrix[COLLIDER_FPLAYER][COLLIDER_ENEMY] = false;

	matrix[COLLIDER_DEATH][COLLIDER_WALL] = false;
	matrix[COLLIDER_DEATH][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_DEATH][COLLIDER_ENDOFLEVEL] = false;
	matrix[COLLIDER_DEATH][COLLIDER_FPLAYER] = false;
	matrix[COLLIDER_DEATH][COLLIDER_DEATH] = false;
	matrix[COLLIDER_DEATH][COLLIDER_ENEMY] = false;

	matrix[COLLIDER_ENEMY][COLLIDER_WALL] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_ENDOFLEVEL] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_FPLAYER] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_DEATH] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = true;
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

	return true;
}

bool j1Collision::Update(float d_time)
{
	//Calculate future collisions
	Collider* c1;
	Collider* c2;

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

			//check if a certain collider will collide with future player collider
			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}
	

	//check collisions with future collider
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

Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	LOG("Collider created successfully. Attributes: x=%d, y=%d, w=%d; h=%d", ret->rect.x, ret->rect.y, ret->rect.w, ret->rect.h);

	return ret;
}

bool j1Collision::EraseCollider(Collider * collider)
{
	return false;
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
		case COLLIDER_WALL: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_ENDOFLEVEL: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_FPLAYER: 
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_DEATH: // gray
			App->render->DrawQuad(colliders[i]->rect, 136, 136, 136, alpha);
			break;
		case COLLIDER_ENEMY:
			App->render->DrawQuad(colliders[i]->rect, 170, 0, 255, alpha);
			break;

		}
	}
}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x + rect.w >= r.x && rect.x <= r.x + r.w && rect.y + rect.h >= r.y && rect.y <= r.y + r.h);
}


bool j1Collision::PostUpdate()
{
	return true;
}

