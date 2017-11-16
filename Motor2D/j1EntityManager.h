#ifndef _ENTITY_MANAGER_
#define _ENTITY_MANAGER_

#include "p2List.h"
#include "j1Module.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Flying_Enemy.h"

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
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	void SetToStart();
private:
	
public:

	p2List<Entity*> entity_list;
	Player* player1;

private:
};

#endif // !_ENTITY_MANAGER_
