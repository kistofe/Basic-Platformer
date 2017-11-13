#include "j1EntityManager.h"



j1EntityManager::j1EntityManager()
{
}


j1EntityManager::~j1EntityManager()
{
}

bool j1EntityManager::Update(float d_time)
{
	accumulated_time += d_time;
	if (accumulated_time >= update_ms_cycle)
		do_logic = true;

	//UpdateAll(d_time, do_logic)

	if (do_logic == true)
	{
		accumulated_time = 0.0f;
		do_logic = false;
	}
	
	return true;
}

Entity* j1EntityManager::CreateEntity(Entity::EntityType type)
{
	Entity* ret = nullptr;

	switch (type)
	{
		case Entity::EntityType::PLAYER:	ret = new Player();	break;
	//	case Entity::EntityType::ENEMY:	ret = new Enemy();	break;
	}
	   
	if (ret != nullptr)
		entities.add(ret);


	return ret;
}

bool j1EntityManager::DestroyEntity(Entity * entity)
{
	return true;
}
