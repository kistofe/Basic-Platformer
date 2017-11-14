#include "j1EntityManager.h"


j1EntityManager::j1EntityManager()
{
	name.create("entity_manager");
}


j1EntityManager::~j1EntityManager()
{
}

bool j1EntityManager::Awake(pugi::xml_node& data)
{
	bool ret = true;

	Player* player = (Player*) CreateEntity(Entity::EntityType::PLAYER);
		
	p2List_item<Entity*>* entity_iterator;
	entity_iterator = entity_list.start;

	while (entity_iterator != NULL && ret == true)
	{
		ret = entity_iterator->data->Awake(data.child(entity_iterator->data->name.GetString()));
		entity_iterator = entity_iterator->next;
	}

	return ret;
}

bool j1EntityManager::Start()
{
	bool ret = true;

	p2List_item<Entity*>* entity_iterator;
	entity_iterator = entity_list.start;

	while (entity_iterator != NULL && ret == true)
	{
		ret = entity_iterator->data->Start();
		entity_iterator = entity_iterator->next;
	}

	return ret;
}

bool j1EntityManager::PreUpdate(float d_time)
{
	bool ret = true;

	p2List_item<Entity*>* entity_iterator;
	entity_iterator = entity_list.start;

	while (entity_iterator != NULL && ret == true)
	{
		ret = entity_iterator->data->PreUpdate(d_time);
		entity_iterator = entity_iterator->next;
	}

	return ret;
}

bool j1EntityManager::Update(float d_time)
{
	bool ret = true;

	p2List_item<Entity*>* entity_iterator;
	entity_iterator = entity_list.start;

	while (entity_iterator != NULL && ret == true)
	{
		ret = entity_iterator->data->Update(d_time);
		entity_iterator = entity_iterator->next;
	}

	return ret;
}

bool j1EntityManager::PostUpdate()
{
	bool ret = true;

	p2List_item<Entity*>* entity_iterator;
	entity_iterator = entity_list.start;

	while (entity_iterator != NULL && ret == true)
	{
		ret = entity_iterator->data->PostUpdate();
		entity_iterator = entity_iterator->next;
	}

	return ret;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;

	p2List_item<Entity*>* entity_iterator;
	entity_iterator = entity_list.end;

	while (entity_iterator != NULL && ret == true)
	{
		ret = entity_iterator->data->CleanUp();
		entity_iterator = entity_iterator->prev;
	}

	return ret;
}

Entity* j1EntityManager::CreateEntity(Entity::EntityType type)
{
	Entity* ret = nullptr;

	switch (type)
	{
		case Entity::EntityType::PLAYER:	ret = new Player();	break;
		case Entity::EntityType::ENEMY:		ret = new Enemy(); break;
	}
	   
	if (ret != nullptr)
		entity_list.add(ret);


	return ret;
}

bool j1EntityManager::DestroyEntity(Entity * entity)
{
	return true;
}
