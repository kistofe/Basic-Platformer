#include "j1App.h"
#include "Entity.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1SceneSwitch.h"
#include "j1InGameScene.h"

Entity::Entity(EntityType type) : type(type)
{}

Entity::~Entity()
{}

void Entity::SetToStart()
{
	position.x = original_position.x;
	position.y = original_position.y;
	speed.x = 0;
	speed.y = 0;
	current_animation = default_animation;
}

void Entity::Draw()
{
	if (App->ingamescene->paused)
	{
		if (this->temp_speed == 0)
			this->temp_speed = current_animation->speed;
		current_animation->speed = 0;
	}

	if (!App->ingamescene->paused && current_animation->speed == 0)
		current_animation->speed = this->temp_speed;
	
	if (facing_right)
		App->render->Blit(current_tex, position.x, position.y, &(current_animation->GetCurrentFrame()), 1.0f, 0.0, 2147483647, 2147483647);

	else
		App->render->Blit(current_tex, position.x, position.y, &(current_animation->GetCurrentFrame()), 1.0f, 0.0, 2147483647, 2147483647, true);
}

void Entity::Move()
{
	position.x += speed.x;
	position.y += speed.y;
}

Collider* Entity::GetCollider()
{
	return collider;
}

float Entity::GetAnimationSpeed(Animation* current_anim)
{
	return current_anim->speed;
}
