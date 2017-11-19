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

	Ground_Enemy(uint x, uint y);
	~Ground_Enemy();

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

	void SetToStart();

	void MoveTowardsPlayer(float d_time);
	
private:
public:
private:
	Animation idle;
	Animation run;
	Animation bite;
		
};
#endif 
