#ifndef __j1PLAYER__
#define __j1PLAYER__

#include "Animation.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "j1Collision.h"
#include "Entity.h"

#include "SDL/include/SDL.h"

struct Object;

class Player : public Entity
{
public:
	Player(uint x, uint y);

	//Destructor
	~Player();
	
	bool Start();

	bool PreUpdate(float d_time);

	bool Update(float d_time);

	bool CleanUp();

	//Loads player pos
	bool Load(pugi::xml_node&);

	//Saves player pos
	bool Save(pugi::xml_node&) const;

	void OnCollision(Collider* c1, Collider* c2);
		
	void SetAnimations();

	void CreateAnimationPushBacks();
	
private:
	void SetSpeed(float d_time);
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
	bool is_grounded		= true;

	float		jumping_speed;
		
	//Player SFX
	p2SString jumping_sfx_source;
	p2SString landing_sfx_source;
	
	uint jumping_sfx;
	uint landing_sfx;

private:
	Object* player;
};

#endif