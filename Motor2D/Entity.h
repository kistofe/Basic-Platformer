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
	virtual void Move() { return; }
	virtual bool Save(pugi::xml_node&) { return true; };
	virtual bool Load(pugi::xml_node&) const { return true; };
	virtual void SetToStart() { return; };
	virtual void SetSpeed(float d_time) { return; };
	virtual void SetAnimations() { return; };
	virtual void CreateAnimationPushBacks() { return; };
	

public:

	p2SString name;

protected:

	Collider*	collider;
	Collider*	future_collider;
		
	iPoint		position = { 0, 0 };
	fPoint		speed = { 0, 0 };
	iPoint		collider_offset;

	EntityType	type;
	SDL_Texture* texture = NULL;
	Animation*	current_animation = nullptr;

	p2SString	death_sfx_source;

	bool		facing_right = true;
	float		moving_speed;
	uint		death_sfx;

};
#endif
