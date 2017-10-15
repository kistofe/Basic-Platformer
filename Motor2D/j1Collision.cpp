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

	return true;
}

bool j1Collision::Update(float dt)
{
	//Calculate future collisions
	Collider* c1;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		//check if a certain collider will collide with future player collider
		if (SDL_HasIntersection(&c1->rect, &App->player->future_player_col->rect))
		{
			//if does, call will_collide
			if(matrix[c1->type][App->player->future_player_col->type])
				will_collide(c1, App->player->future_player_col);
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

bool j1Collision::will_collide(Collider* c1, Collider* c2)
{
	bool ret = true;

	if (c1->type == COLLIDER_WALL)
	{
		SDL_Rect intersect_col;
		if(SDL_IntersectRect(&c1->rect, &c2->rect, &intersect_col));
		//future player collider and a certain collider have collided
		{
			if (App->player->player_speed.y == 0)//case y = 0 
			{
				if (App->player->player_speed.x > 0)//case +x
				{
					App->player->player_speed.x -= intersect_col.w;
				}

				else//case -x
				{
					App->player->player_speed.x += intersect_col.w;
				}

			}
			else if (App->player->player_speed.x == 0)// case x = 0
			{
				if (App->player->player_speed.y > 0)//case y+
				{
					App->player->player_speed.y -= intersect_col.h;
				}

				else//case -y
				{
					App->player->player_speed.y += intersect_col.h;
				}
			}
			else if (App->player->player_speed.x != 0 && App->player->player_speed.y != 0)//case y != 0 && x != 0
			{
				if (App->player->player_speed.x > 0 && App->player->player_speed.y > 0)//case +y/+x
				{
					App->player->player_speed.x -= intersect_col.w;
					App->player->player_speed.y -= intersect_col.h;
				}

				else if (App->player->player_speed.x < 0 && App->player->player_speed.y < 0)//case -x/-y
				{
					App->player->player_speed.x += intersect_col.w;
					App->player->player_speed.y += intersect_col.h;
				}

				else if (App->player->player_speed.x < 0 && App->player->player_speed.y > 0)//case -x/+y
				{
					App->player->player_speed.x += intersect_col.w;
					App->player->player_speed.y -= intersect_col.h;
				}

				else if (App->player->player_speed.x > 0 && App->player->player_speed.y < 0)//case +x/-y
				{
					App->player->player_speed.x -= intersect_col.w;
					App->player->player_speed.y += intersect_col.h;
				}
			}
		}
		App->player->future_player_col->SetPos(App->player->future_player_col->rect.x - (App->player->original_speed.x - App->player->player_speed.x), App->player->future_player_col->rect.y - (App->player->original_speed.y - App->player->player_speed.y));
	}

	if (c2->type == COLLIDER_ENDOFLEVEL)
	{
		App->sceneswitch->FadeToBlack();
	}
	
	
	return ret;
}

bool j1Collision::PostUpdate()
{
	
		return true;
}
