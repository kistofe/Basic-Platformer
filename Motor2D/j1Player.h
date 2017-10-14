#ifndef __j1PLAYER__
#define __j1PLAYER__

#include "p2List.h"
#include "p2Point.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "j1Module.h"
#include "j1Collision.h"
#include "Animation.h"
#include "j1App.h"
#include "j1Map.h"

#include "SDL/include/SDL.h"

class j1Player : public j1Module
{
public:
	j1Player();

	//Destructor
	~j1Player();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool CleanUp();

	//Loads player pos
	bool Load(pugi::xml_node&);

	//Saves player pos
	bool Save(pugi::xml_node&) const;
	
	//Raycast function to detect if player collides
	bool PredictCollision(const SDL_Rect*, const SDL_Rect*);

	void SetCorrectVel(const SDL_Rect*);

	SDL_Rect TransformToRect(Object*);

private:

	void SetSpeed();

public:

	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation idle;
	Animation walking;
	Animation running;
	Animation jump;

	bool facing_right = true;

	iPoint		player_pos;
	fPoint		player_speed;

	Collider*	player_collider;

private:
	

};

#endif
