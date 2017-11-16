#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "Enemy.h"


Enemy::Enemy(EntityType type) : Entity(type)
{
	name.create("enemy");
}

Enemy::~Enemy()
{
}

bool Enemy::Awake(pugi::xml_node& data)
{
	return true;
}

bool Enemy::Start()
{
	return true;
}

bool Enemy::PreUpdate(float d_time)
{
	return true;
}

bool Enemy::Update(float d_time)
{
	return true;
}

bool Enemy::CleanUp()
{
	return true;
}

void Enemy::ChangeSpeed(iPoint newspeed, float d_time)
{
	speed.x = newspeed.x * d_time * 100;
	speed.y = newspeed.y * d_time * 100;
}
