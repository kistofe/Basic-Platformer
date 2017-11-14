#ifndef _ENTITY_MANAGER_
#define _ENTITY_MANAGER_

#include "p2List.h"
#include "j1Module.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"

class j1EntityManager : public j1Module
{
public:
	j1EntityManager();
	~j1EntityManager();

	bool Awake(pugi::xml_node& data);
	bool Start();
	bool PreUpdate(float d_time);
	bool Update(float d_time);
	bool PostUpdate();
	bool CleanUp();

	Entity* CreateEntity(Entity::EntityType type);
	bool DestroyEntity(Entity* entity);
	
private:
	
public:

	p2List<Entity*> entity_list;

private:
};

#endif // !_ENTITY_MANAGER_
