#ifndef _FLYING_ENEMY_
#define _FLYING_ENEMY_

#include "Animation.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "j1Collision.h"
#include "Enemy.h"

struct Object;


class Flying_Enemy : public Enemy
{
public:

	Flying_Enemy(uint x, uint y);
	~Flying_Enemy();

	bool Start();
	bool PreUpdate(float d_time);
	bool Update(float d_time);
	bool CleanUp();

	//Loads player pos
	bool Load(pugi::xml_node&);

	//Saves player pos
	bool Save(pugi::xml_node&) const;

	void MoveTowardsPlayer(float d_time);

	void OnCollision(Collider* c1, Collider* c2);

	void SetAnimations();

	void CreateAnimationPushBacks();


private:

	Animation fly;
	Animation bite;

	Object* flying_enemy;

};
#endif 
