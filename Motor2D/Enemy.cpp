#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "Enemy.h"

Enemy::Enemy() : Entity(Entity::EntityType::ENEMY)
{
	name.create("enemy");
}

Enemy::~Enemy()
{
}

bool Enemy::Awake(pugi::xml_node & data)
{
	return false;
}

bool Enemy::Start()
{
	return false;
}

bool Enemy::PreUpdate(float d_time)
{
	return false;
}

bool Enemy::Update(float d_time)
{
	return false;
}

bool Enemy::CleanUp()
{
	return false;
}
