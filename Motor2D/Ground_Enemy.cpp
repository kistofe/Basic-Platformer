#include "Ground_Enemy.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Map.h"


Ground_Enemy::Ground_Enemy() : Enemy(Entity::EntityType::GROUND_ENEMY)
{
	name.create("ground_enemy");
	CreateAnimationPushBacks();
}


Ground_Enemy::~Ground_Enemy()
{
}

bool Ground_Enemy::Start()
{
	LOG("Loading Ground Enemy");

	texture = App->tex->Load("images/Ground Enemy.png");
	ground_enemy = App->map->GetObj("Ground_Enemy");
	position.create(ground_enemy->x, ground_enemy->y);
	SetProperties(ground_enemy);

	//Creating Colliders
	collider = App->collision->AddCollider({ position.x + collider_offset.x, position.y + collider_offset.y, 30, 30 }, COLLIDER_ENEMY, this);
	future_collider = App->collision->AddCollider({ collider->rect.x, collider->rect.y, 30, 30 }, COLLIDER_FPLAYER, this);

	//current_animation = &fly;

	return true;
}

bool Ground_Enemy::PreUpdate()
{
	//Call ChangeSpeed()
	//Update Future collider

	return true;
}

bool Ground_Enemy::Update()
{
	//Check bool alive (only enter if the enemy is still alive
	//Call Draw()
	//Call Move()
	//Call SetAnimations()
	return true;
}

bool Ground_Enemy::CleanUp()
{
	//Unload Texture
	App->tex->UnLoad(texture);
	return true;
}

bool Ground_Enemy::Load(pugi::xml_node &)
{
	//Load position
	//Load velocity
	//Load status (dead or alive)
	return true;
}

bool Ground_Enemy::Save(pugi::xml_node &) const
{
	//Save position
	//Save velocity
	//Save status
	return true;
}

void Ground_Enemy::OnCollision(Collider * c1, Collider * c2)
{
	//Check collision with scenario
	//Check collision with player
}


void Ground_Enemy::SetAnimations()
{
	//Check case and adjust animation
}

void Ground_Enemy::CreateAnimationPushBacks()
{
	current_animation = NULL;

}

void Ground_Enemy::SetProperties(Object * entity)
{
	p2List_item<Properties::Object_property*>* iterator = entity->properties.obj_property_list.start;
	while (iterator)
	{
		if (moving_speed == 0)
			moving_speed = iterator->data->moving_speed;
		if (collider_offset.x == 0)
			collider_offset.x = iterator->data->collider_offset.x;
		if (collider_offset.y == 0)
			collider_offset.y = iterator->data->collider_offset.y;
		if (death_sfx_source.Length() == 0)
			death_sfx_source = iterator->data->death_sfx_source;

		iterator = iterator->next;
	}
}

