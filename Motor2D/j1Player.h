#ifndef __j1PLAYER__
#define __j1PLAYER__

#include "Animation.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "j1Module.h"
#include "j1Collision.h"
#include "j1App.h"
#include "j1Map.h"

#include "SDL/include/SDL.h"

class j1Player : public j1Module
{
public:
	j1Player();

	//Destructor
	~j1Player();

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
	

private:

	void SetSpeed();
	void SetAnimations();
	void CreateAnimationPushBacks();

public:

	//Player Animations
	Animation* current_animation = nullptr;
	Animation idle;
	Animation run;
	Animation jump;
	Animation double_jump;
	Animation win;
	Animation falling;
	Animation damaged;
	Animation death;

	//Player Status
	uint jumps_left			= 2;
	bool facing_right		= true;
	bool is_grounded		= true;

	iPoint		position		= { 0,0 };
	fPoint		speed			= { 0,0 };
	float		moving_speed;
	iPoint		collider_offset;
	

	//Player collider and Future Player Collider
	Collider*	collider;
	Collider*	future_collider;

	//Player Texture
	SDL_Texture* player_tex = nullptr;

	//Player SFX
	p2SString jumping_sfx_source;
	p2SString landing_sfx_source;
	uint jumping_sfx;
	uint landing_sfx;

private:

	
};

#endif
