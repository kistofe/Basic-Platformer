#ifndef __ENEMY__
#define __ENEMY__

#include "Animation.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "Entity.h"

#include "SDL/include/SDL.h"

class Enemy : public Entity
{
public:


	Enemy(EntityType type);

	//Destructor
	~Enemy();

	bool Awake(pugi::xml_node& data);

	bool Start();

	bool PreUpdate(float d_time);

	bool Update(float d_time);

	bool CleanUp();

	void ChangeSpeed(iPoint newspeed, float d_time);
	//Something like a Path Function to apply Pathfinding?
	

public:
	//Maybe an enemy list?
	
protected:

	bool alive = true;
	


};

#endif
