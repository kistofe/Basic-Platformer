#ifndef _ENTITY_
#define _ENTITY_

#include "Animation.h"
#include "p2Defs.h"
#include "p2Point.h"
#include "j1Module.h"

#include "SDL\include\SDL.h"

enum EntityType
{
	PLAYER,
	ENEMY
};

struct Entity
{
	iPoint position;
	fPoint speed;
	EntityType type;
	SDL_Texture* texture;
	Collider* collider;
	Animation* current_animation = nullptr;
};

class j1Entity
{
public:
	j1Entity();
	~j1Entity();

	virtual void Update() {};
	virtual void Draw() {};
	virtual void HandleInput() {};
	
};
#endif
