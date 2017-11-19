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

	
	bool Start();

	bool PreUpdate(float d_time);

	bool Update(float d_time);

	bool CleanUp();

	void ChangeSpeed(iPoint newspeed, float d_time);
	
	virtual void MoveTowardsPlayer() { return; };

public:
	
protected:

};

#endif
