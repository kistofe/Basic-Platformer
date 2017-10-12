#ifndef __j1COLLISION_H__
#define __j1COLLISION_H__

#include "j1Module.h"
#include "j1Render.h"

#define MAX_COLLIDERS 150

// ----------------------------------------------------
enum COLLIDER_TYPE
{
	COLLIDER_PLAYER,
	COLLIDER_WALL,
	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool PredictCollision(const SDL_Rect& r) const;
};

class j1Collision : public j1Module
{
public:

	j1Collision();

	~j1Collision();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


	// Method to add new colliders (usually called on map and player generation)
	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);

	// Method to delete colliders on CleanUps
	bool EraseCollider(Collider* collider);

	// Method to draw the colliders on screen in debug mode (F1)
	void DebugDraw();

private:

	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = false;
};


#endif // __j1COLLISION_H__