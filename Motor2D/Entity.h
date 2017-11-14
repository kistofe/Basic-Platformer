#ifndef _ENTITY_
#define _ENTITY_

#include "Animation.h"
#include "p2Defs.h"
#include "p2Point.h"
#include "p2SString.h"

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

	virtual bool Awake(pugi::xml_node&) { return true; };
	virtual bool Start() { return true; };
	virtual bool PreUpdate(float d_time) { return true; };
	virtual bool Update(float d_time) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };

	virtual void Draw() { return; };
	virtual void OnCollision(Collider* c1, Collider* c2) { return; };
	virtual bool Save(pugi::xml_node&) { return true; };
	virtual bool Load(pugi::xml_node&) const { return true; };

protected:

public:

	p2SString name;
protected:

	iPoint position = { 0, 0 };
	fPoint speed = { 0, 0 };
	EntityType type;
	SDL_Texture* texture;
	Collider* collider;
	Animation* current_animation = nullptr;

	bool facing_right = true;
	float		moving_speed;

};
#endif
