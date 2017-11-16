#include "Ground_Enemy.h"
#include "j1App.h"
#include "j1Textures.h"


Ground_Enemy::Ground_Enemy() : Enemy(Entity::EntityType::GROUND_ENEMY)
{
}


Ground_Enemy::~Ground_Enemy()
{
}

bool Ground_Enemy::Start()
{
	//Read position from map?
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

