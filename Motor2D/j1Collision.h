#ifndef __j1Collision_H__
#define __j1Collision_H__

#define MAX_COLLIDERS 500

#include "j1Module.h"
#include "j1Render.h"

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_WALL,
	COLLIDER_PLAYER,
	COLLIDER_FPLAYER,
	COLLIDER_ENDOFLEVEL,
	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type) :
		rect(rectangle),
		type(type)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

};

class j1Collision : public j1Module
{
public:

	j1Collision();
	~j1Collision();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type);
	bool EraseCollider(Collider* collider);
	void DebugDraw();

private:

	bool Avoid_Collision(Collider*, Collider*);

private:

	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = false;
};

#endif // __j1Collision_H__