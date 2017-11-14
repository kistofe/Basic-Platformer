#ifndef __j1PLAYER__
#define __j1PLAYER__

#include "Animation.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "Entity.h"

#include "SDL/include/SDL.h"

class Player : public Entity
{
public:
	Player();

	//Destructor
	~Player();

	bool Awake(pugi::xml_node& data);

	bool Start();

	bool PreUpdate();

	bool Update(float d_time);

	bool CleanUp();

	//Loads player pos
	bool Load(pugi::xml_node&);

	//Saves player pos
	bool Save(pugi::xml_node&) const;

	void OnCollision(Collider* c1, Collider* c2);

	void SetToStart();
	

private:

	void SetSpeed();
	void SetAnimations();
	void CreateAnimationPushBacks();
	void SetCameraToPlayer();

public:

	//Player Animations
	Animation idle;
	Animation run;
	Animation jump;
	Animation double_jump;
	Animation win;
	Animation fall;
	Animation damaged;
	Animation death;

	//Player Status
	uint jumps_left			= 2;
	bool facing_right		= true;
	bool is_grounded		= true;

	float		moving_speed;
	float		jumping_speed;
	iPoint		collider_offset;
	

	//Player collider and Future Player Collider
	Collider*	future_collider;

	//Player Texture
	
	//Player SFX
	p2SString jumping_sfx_source;
	p2SString landing_sfx_source;
	p2SString death_sfx_source;
	uint jumping_sfx;
	uint landing_sfx;
	uint death_sfx;

private:

	
};

#endif
