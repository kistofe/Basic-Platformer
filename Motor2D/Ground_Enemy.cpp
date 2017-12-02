#include "Ground_Enemy.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"
#include "j1Map.h"


Ground_Enemy::Ground_Enemy(uint x, uint y) : Enemy(Entity::EntityType::GROUND_ENEMY)
{
	name.create("ground_enemy");
	CreateAnimationPushBacks();

	pugi::xml_document config_file;
	pugi::xml_node config;
	pugi::xml_node data;

	config = App->LoadConfig(config_file);
	data = config.child("entity_manager").child("flying_enemy");

	position.x			= x;
	position.y			= y;

	//Saving original position to later restart it
	original_position.x = x;
	original_position.y = y;

	default_animation = &idle;

	current_tex			= App->tex->Load("images/Ground Enemy.png");

	collider_offset.x	= data.child("collider_offset_x").attribute("value").as_int();
	collider_offset.y	= data.child("collider_offset_y").attribute("value").as_int();


}


Ground_Enemy::~Ground_Enemy()
{
}

bool Ground_Enemy::Start()
{
	LOG("Loading Ground Enemy");
	
	//Creating Colliders
	collider			= App->collision->AddCollider({ position.x + collider_offset.x, position.y + collider_offset.y, 70, 40 }, COLLIDER_ENEMY, this);
	future_collider		= App->collision->AddCollider({ collider->rect.x, collider->rect.y, 70, 40 }, COLLIDER_FUTURE, this);

	current_animation	= &idle;

	return true;
}

bool Ground_Enemy::PreUpdate(float d_time)
{
	//Call ChangeSpeed()
	//Update Future collider
	future_collider->SetPos((collider->rect.x + speed.x), (collider->rect.y + speed.y));
	facing_right = false;
	return true;
}

bool Ground_Enemy::Update(float d_time)
{
	//Check bool alive (only enter if the enemy is still alive
	//Call Draw()

	//Call SetAnimations()
	MoveTowardsPlayer(d_time);

	Move();
	
	//Update Collider Position-------------------------------------
	collider->SetPos(position.x + collider_offset.x, position.y + collider_offset.y);
	//Update Blit -------------------------------------------------
	Draw();
	return true;
}

bool Ground_Enemy::CleanUp()
{
	//Unload Texture
	App->tex->UnLoad(current_tex);

	return true;
}

bool Ground_Enemy::Load(pugi::xml_node& data)
{
	position.x		= data.child("position").attribute("x").as_int();
	position.y		= data.child("position").attribute("y").as_int();
	speed.x			= data.child("velocity").attribute("x").as_float();
	speed.y			= data.child("velocity").attribute("y").as_float();
	facing_right	= data.child("status").child("facing_right").attribute("value").as_bool();
	
	return true;
}

bool Ground_Enemy::Save(pugi::xml_node& data) const
{
	pugi::xml_node ground_enemy = data.append_child("ground_enemy");
	pugi::xml_node pos = ground_enemy.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;

	pugi::xml_node vel = ground_enemy.append_child("velocity");

	vel.append_attribute("x") = speed.x;
	vel.append_attribute("y") = speed.y;

	pugi::xml_node status = ground_enemy.append_child("status");

	status.append_child("facing_right").append_attribute("value") = facing_right;
	
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

	idle.PushBack({ 0,0,96, 52 });
	idle.PushBack({ 96,0,96, 52 });
	idle.PushBack({ 192,0,96, 52 });
	idle.PushBack({ 288,0,96, 52 });
	idle.loop = true;
	idle.speed = 0.2f;

	run.PushBack({ 0, 52, 96, 52 });
	run.PushBack({ 96, 52, 96, 52 });
	run.PushBack({ 192, 52, 96, 52 });
	run.PushBack({ 288, 52, 96, 52 });
	run.PushBack({ 384, 52, 96, 52 });
	run.PushBack({ 480, 52, 96, 52 });
	run.loop = true;
	run.speed = 0.4f;

	bite.PushBack({ 0, 104, 96, 52 });
	bite.PushBack({ 96, 104, 96, 52 });
	bite.PushBack({ 192, 104, 96, 52 });
	bite.PushBack({ 288, 104, 96, 52 });
	bite.loop = false;
	bite.speed = 0.4f;
}

void Ground_Enemy::MoveTowardsPlayer(float d_time)
{
	
}


