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

enum EnemyType
{
	GROUNDED,
	FLYING
};

	Enemy();

	//Destructor
	~Enemy();

	bool Awake(pugi::xml_node& data);

	bool Start();

	bool PreUpdate(float d_time);

	bool Update(float d_time);

	bool CleanUp();

	
private:


public:

	
private:


};

#endif
