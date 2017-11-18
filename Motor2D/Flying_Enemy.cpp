#include "Flying_Enemy.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Pathfinding.h"
#include "j1EntityManager.h"
#include "j1Scene.h"
#include "j1Map.h"


Flying_Enemy::Flying_Enemy() : Enemy(Entity::EntityType::FLYING_ENEMY)
{
	name.create("flying_enemy");
	CreateAnimationPushBacks();
}


Flying_Enemy::~Flying_Enemy()
{
}


bool Flying_Enemy::Start()
{
	LOG("Loading Flying Enemy");

	texture = App->tex->Load("images/Flying Enemy.png");
	flying_enemy = App->map->GetObj("Flying_Enemy");
	position.create(flying_enemy->x, flying_enemy->y);
	SetProperties(flying_enemy);

	//Creating Colliders
	collider = App->collision->AddCollider({ position.x + collider_offset.x, position.y + collider_offset.y, 30, 30 }, COLLIDER_ENEMY, this);
	future_collider = App->collision->AddCollider({ collider->rect.x, collider->rect.y, 30, 30 }, COLLIDER_FPLAYER, this);

	current_animation = &fly;

	return true;
}

bool Flying_Enemy::PreUpdate(float d_time)
{
	//Call ChangeSpeed()
	//Update Future collider

	return true;
}

bool Flying_Enemy::Update(float d_time)
{
	//Check bool alive (only enter if the enemy is still alive
	//Call SetAnimations()
	MoveTowardsPlayer(d_time);

	//Update Position ---------------------------------------------	
	Move();
	//Update Collider Position-------------------------------------
	collider->SetPos(position.x + collider_offset.x, position.y + collider_offset.y);
	//Update Blit -------------------------------------------------
	Draw();
	return true;
}

bool Flying_Enemy::CleanUp()
{
	//Unload Texture
	App->tex->UnLoad(texture);
	return true;
}

bool Flying_Enemy::Load(pugi::xml_node& data)
{
	//Load position
	//Load velocity
	//Load status (dead or alive)
	return true;
}

bool Flying_Enemy::Save(pugi::xml_node& data) const
{
	//Save position
	//Save velocity
	//Save status
	pugi::xml_node flying_enemy = data.append_child("flying_enemy");
	pugi::xml_node pos = flying_enemy.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;

	pugi::xml_node vel = flying_enemy.append_child("velocity");

	vel.append_attribute("x") = speed.x;
	vel.append_attribute("y") = speed.y;

	pugi::xml_node status = flying_enemy.append_child("status");
	status.append_child("facing_right").append_attribute("value") = facing_right;

	return true;
}

void Flying_Enemy::MoveTowardsPlayer(float d_time)
{
	iPoint tile_to_go = App->pathfinding->GetNextTile(App->map->WorldToMap(position.x, position.y), App->map->WorldToMap(App->scene->player1->position.x, App->scene->player1->position.y));
	iPoint position_in_world = App->map->WorldToMap(position.x, position.y);
	// create a vector from the current position to the tile that it has to go to
	fPoint distance;
	distance.create(tile_to_go.x - position_in_world.x, tile_to_go.y - position_in_world.y);

	// create a speed vector
	iPoint speed_to_go;
	speed_to_go.create(distance.x, distance.y);

	ChangeSpeed(speed_to_go, d_time);
}

void Flying_Enemy::OnCollision(Collider * c1, Collider * c2)
{
	//Maybe flying enemy does not collide with scenario
	//Check collision with player
}


void Flying_Enemy::SetAnimations()
{
	//Check case and adjust animation
		//While player does not reach a certain distance, the enemy should stay in its fly animation
}

void Flying_Enemy::CreateAnimationPushBacks()
{
	current_animation = NULL;

	//Normal flying animation
	fly.PushBack({ 0, 0, 64, 55 });
	fly.PushBack({ 64, 0, 64, 55 });
	fly.PushBack({ 128, 0, 64, 55 });
	fly.loop = true;
	fly.speed = 0.35f;

	//Animation when enemy gets close to the player
	bite.PushBack({ 0, 55, 64, 55 });
	bite.PushBack({ 64, 55, 64, 55 });
	bite.PushBack({ 128, 55, 64, 55 });
	bite.PushBack({ 192, 55, 64, 55 });
	bite.PushBack({ 256, 55, 64, 55 });
	bite.PushBack({ 320, 55, 64, 55 });
	bite.loop = false;
	bite.speed = 0.3f;
}

void Flying_Enemy::SetProperties(Object* entity)
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
