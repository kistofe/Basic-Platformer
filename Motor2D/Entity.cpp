#include "j1App.h"
#include "Entity.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1SceneSwitch.h"
#include "j1Scene.h"



Entity::Entity(EntityType type) : type(type)
{}


Entity::~Entity()
{
}

bool Entity::AddEntity(EntityType type, int x, int y)
{
	bool ret = false;
	
	p2List_item<ObjGroup*>* objgroup_iterator = App->map->data.objgroup.start;
	while (objgroup_iterator)
	{
		p2List_item<Object*>* obj_iterator = objgroup_iterator->data->object.start;
		while (obj_iterator)
		{
			
		}
		objgroup_iterator = objgroup_iterator->next;
	}

	return ret;
}

void Entity::SetToStart()
{
	//Loop to check current map
	//position.x = App->map->data.object.start->data->x;
//	position.y = App->map->data.object.start->data->y;
	speed.x = 0;
	speed.y = 0;
	
}


void Entity::Draw()
{
	if (facing_right)
		App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame()));

	else
		App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame()), 1.0F, 0.0, 2147483647, 2147483647, true);
}

void Entity::Move()
{
	position.x += speed.x;
	position.y += speed.y;
}