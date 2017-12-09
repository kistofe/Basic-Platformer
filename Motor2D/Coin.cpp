#include "Coin.h"



Coin::Coin(uint x, uint y) : Entity(Entity::EntityType::COIN)
{
	position.x = x;
	position.y = y;
}


Coin::~Coin()
{
}

bool Coin::Start()
{
	return false;
}

bool Coin::Update()
{
	return false;
}

bool Coin::CleanUp()
{
	return false;
}

bool Coin::Load(pugi::xml_node &)
{
	return false;
}

bool Coin::Save(pugi::xml_node &) const
{
	return false;
}

void Coin::SetAnimations()
{
}

void Coin::CreateAnimationPushBacks()
{
}
