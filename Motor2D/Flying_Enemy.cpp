#include "Flying_Enemy.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Pathfinding.h"
#include "j1EntityManager.h"
#include "j1Scene.h"
#include "j1Map.h"

#include "Brofiler\Brofiler.h"


Flying_Enemy::Flying_Enemy(uint x, uint y) : Enemy(Entity::EntityType::FLYING_ENEMY)
{
	name.create("flying_enemy");
	CreateAnimationPushBacks();

	pugi::xml_document config_file;
	pugi::xml_node config;
	pugi::xml_node data;

	config				= App->LoadConfig(config_file);
	data				= config.child("entity_manager").child("flying_enemy");

	position.x			= x;
	position.y			= y;

	//Saving original position to later restart it
	original_position.x = x;
	original_position.y = y;

	default_animation	= &fly;

	current_tex			= App->tex->Load("images/Flying Enemy.png");

	collider_offset.x	= data.child("collider_offset_x").attribute("value").as_int();
	collider_offset.y	= data.child("collider_offset_y").attribute("value").as_int();

	
}


Flying_Enemy::~Flying_Enemy()
{
}


bool Flying_Enemy::Start()
{
	LOG("Loading Flying Enemy");
	
	//Creating Colliders
	collider			= App->collision->AddCollider({ position.x + collider_offset.x, position.y + collider_offset.y, 30, 30 }, COLLIDER_ENEMY, this);
	future_collider		= App->collision->AddCollider({ collider->rect.x, collider->rect.y, 30, 30 }, COLLIDER_FUTURE, this);

	current_animation	= &fly;
	
	return true;
}

bool Flying_Enemy::PreUpdate(float d_time)
{
	//Call ChangeSpeed()

	//Update Future Collider with new speed
	future_collider->SetPos((collider->rect.x + speed.x), (collider->rect.y + speed.y));

	return true;
}

bool Flying_Enemy::Update(float d_time)
{
	BROFILER_CATEGORY("Flying_Enemy - Update", Profiler::Color::GoldenRod);
	//Call SetAnimations()
	iPoint position_world	= App->map->WorldToMap(position.x, position.y);
	int distance_to_player	= (position_world.DistanceNoSqrt(App->map->WorldToMap(App->entities->player1->position.x, App->entities->player1->position.y)));

	if (distance_to_player < 230)
		MoveTowardsPlayer(d_time);
	else if (distance_to_player >= 230)
		ChangeSpeed({ 0,0 }, d_time);
		
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
	App->tex->UnLoad(current_tex);
	return true;
}

bool Flying_Enemy::Load(pugi::xml_node& data)
{
	position.x		= data.child("position").attribute("x").as_int();
	position.y		= data.child("position").attribute("y").as_int();
	speed.x			= data.child("velocity").attribute("x").as_float();
	speed.y			= data.child("velocity").attribute("y").as_float();
	facing_right	= data.child("status").child("facing_right").attribute("value").as_bool();
	
	return true;
}

bool Flying_Enemy::Save(pugi::xml_node& data) const
{
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
	iPoint tile_to_go = App->pathfinding->GetNextTile(App->map->WorldToMap(position.x, position.y), App->map->WorldToMap(App->entities->player1->position.x + App->entities->player1->collider_offset.x, App->entities->player1->position.y + App->entities->player1->collider_offset.y)); //pathfind from current position to the position of the player (taking into account the offset of the collider)
	iPoint position_in_world = App->map->WorldToMap(position.x, position.y);
	// create a vector from the current position to the tile that it has to go to
	fPoint distance;
	distance.create(tile_to_go.x - position_in_world.x, tile_to_go.y - position_in_world.y);

	// create a speed vector
	iPoint speed_to_go;
	speed_to_go.create(distance.x, distance.y);

	if (speed_to_go.x > 0)
		facing_right = true;
	else
		facing_right = false;

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

