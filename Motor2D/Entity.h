#ifndef _ENTITY_
#define _ENTITY_

#include "Animation.h"
#include "p2Defs.h"
#include "p2Point.h"

#include "SDL\include\SDL.h"

struct Collider;

class Entity 
{
public:

enum EntityType
{
	PLAYER,
	ENEMY
};

	Entity(EntityType type);
	~Entity();

	virtual void Update() {};
	virtual void Draw() {};
	virtual void OnCollision(Collider* c1, Collider* c2) {};

protected:

public:
	
protected:

	iPoint position = { 0, 0 };
	fPoint speed = { 0, 0 };
	EntityType type;
	SDL_Texture* texture;
	Collider* collider;
	Animation* current_animation = nullptr;

};
#endif
