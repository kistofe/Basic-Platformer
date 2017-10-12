#ifndef __j1PLAYER__
#define __j1PLAYER__

#include "p2List.h"
#include "p2Point.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "j1Module.h"
#include "Animation.h"
#include "j1App.h"
#include "j1Collision.h"

#include "SDL/include/SDL.h"

class j1Player : public j1Module
{
public:
	j1Player();

	//Destructor
	~j1Player();


	bool Start(pugi::xml_node& config);

	bool PreUpdate();

	bool Update(float dt);

	bool CleanUp();

private:

public:

	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation idle;
	Animation walking;
	Animation running;
	Animation jump;
	iPoint position;

	float speed_x = 0;
	float speed_y = 0;

	void SetSpeed();

};

#endif
