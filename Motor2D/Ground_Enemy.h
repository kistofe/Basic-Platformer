#ifndef _GROUND_ENEMY_
#define _GROUND_ENEMY_

#include "Animation.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "j1Collision.h"
#include "Enemy.h"

class Ground_Enemy : public Enemy
{
public:

	Ground_Enemy();
	~Ground_Enemy();

	bool Start();
	bool PreUpdate();
	bool Update();
	bool CleanUp();

	//Loads player pos
	bool Load(pugi::xml_node&);

	//Saves player pos
	bool Save(pugi::xml_node&) const;

	void OnCollision(Collider* c1, Collider* c2);

	void SetAnimations();

	void CreateAnimationPushBacks();

private:

	
};
#endif 
