#ifndef _ENTITY_
#define _ENTITY_

#include "Animation.h"
#include "p2Defs.h"
#include "p2Point.h"
#include "p2List.h"
#include "p2SString.h"

#include "SDL\include\SDL.h"

struct Collider;

class Entity 
{

public:

enum EntityType
{
	PLAYER,
	FLYING_ENEMY,
	GROUND_ENEMY,
};

	Entity(EntityType type);
	~Entity();

	virtual bool Start() { return true; };
	virtual bool PreUpdate(float d_time) { return true; };
	virtual bool Update(float d_time) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };

	virtual void OnCollision(Collider* c1, Collider* c2) { return; };
	virtual bool Save(pugi::xml_node&) const { return true; };
	virtual bool Load(pugi::xml_node&) { return true; };
	virtual void SetAnimations() { return; };
	virtual void CreateAnimationPushBacks() { return; };
	
	void SetToStart();
	void Draw();
	void Move();
	
public:

	p2SString	name;
	iPoint		position = { 0, 0 };
	iPoint		collider_offset;

protected:

	Collider*	collider;
	Collider*	future_collider;
	
	iPoint		original_position;
	fPoint		speed = { 0, 0 };

	EntityType		type;
	SDL_Texture*	current_tex = NULL;
	Animation*		current_animation = nullptr;
	Animation*		default_animation = nullptr;

	p2SString	death_sfx_source;

	bool		facing_right = true;
	uint		death_sfx;

	
};
#endif
