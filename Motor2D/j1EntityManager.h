#ifndef _ENTITY_MANAGER_
#define _ENTITY_MANAGER_

#include "p2List.h"
#include "j1Module.h"
#include "j1Entity.h"

class j1EntityManager
{
public:
	j1EntityManager();
	~j1EntityManager();

	void CreateEntity(EntityType type);
	void DestroyEntity(Entity* entity);
	
private:

public:
	p2List<Entity*> entities;
private:
};

#endif // !_ENTITY_MANAGER_
