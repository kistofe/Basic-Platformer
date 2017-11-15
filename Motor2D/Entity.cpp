#include "j1App.h"
#include "Entity.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1SceneSwitch.h"


Entity::Entity(EntityType type) : type(type)
{
	
}


Entity::~Entity()
{
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

void Entity::SetToStart()
{
	//Loop should be done maybe?
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->sceneswitch->FadeToBlack();

	position.x = App->map->data.object.start->data->x;
	position.y = App->map->data.object.start->data->y;
	speed.x = 0;
	speed.y = 0;
	//current_animation = &idle; ///Not all entities have idle animation :S
}
