#ifndef _ENTITY_MANAGER_
#define _ENTITY_MANAGER_

#include "p2List.h"
#include "j1Module.h"
#include "Entity.h"
#include "Player.h"

class j1EntityManager : public j1Module
{
public:
	j1EntityManager();
	~j1EntityManager();

	bool Update(float d_time);

	Entity* CreateEntity(Entity::EntityType type);
	bool DestroyEntity(Entity* entity);
	
private:
	
	float accumulated_time = 0;
	float update_ms_cycle = 0;

	bool do_logic = false; //bool? or other class?
	
public:

	p2List<Entity*> entities;

private:
};

#endif // !_ENTITY_MANAGER_
