#include "j1EntityManager.h"
#include "j1App.h"

#include "Brofiler\Brofiler.h"


j1EntityManager::j1EntityManager()
{
	name.create("entity_manager");

}


j1EntityManager::~j1EntityManager()
{
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
	BROFILER_CATEGORY("j1EntityManager - PreUpdate", Profiler::Color::Blue);
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
	BROFILER_CATEGORY("j1EntityManager - Update", Profiler::Color::CadetBlue);
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
	BROFILER_CATEGORY("j1EntityManager - PostUpdate", Profiler::Color::Aquamarine);
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
		ret = DestroyEntity(entity_iterator->data);
		entity_iterator = entity_iterator->prev;
	}
	entity_list.clear();
	
	active = false;

	return ret;
}

Entity* j1EntityManager::CreateEntity(Entity::EntityType type, uint x, uint y)
{
	Entity* ret = nullptr;


	switch (type)
	{
		case Entity::EntityType::PLAYER:			ret = new Player(x, y); 
			break;
		case Entity::EntityType::FLYING_ENEMY:		ret = new Flying_Enemy(x, y);
			break;
		case Entity::EntityType::GROUND_ENEMY:		ret = new Ground_Enemy(x, y);
			break;
		case Entity::EntityType::COIN:				ret = new Coin(x, y);
			break;
	}


	if (ret != nullptr)
		entity_list.add(ret);


	return ret;
}

bool j1EntityManager::DestroyEntity(Entity * entity)
{
	bool ret = true;

	if (entity != nullptr)
		delete entity;

	else
		ret = false;

	return ret;
}

bool j1EntityManager::Load(pugi::xml_node & data) 
{

	bool ret = true;

	p2List_item<Entity*>* entity_iterator;
	entity_iterator = entity_list.start;
	pugi::xml_node& player_data = data.child("player");
	pugi::xml_node& fenemy_data = data.child("flying_enemy");
	pugi::xml_node& genemy_data = data.child("ground_enemy");

	while (entity_iterator != NULL && ret == true)
	{
		if (entity_iterator->data->name == "player")
			ret = entity_iterator->data->Load(player_data);

		if (entity_iterator->data->name == "flying_enemy")
		{
			ret = entity_iterator->data->Load(fenemy_data);
			fenemy_data = fenemy_data.next_sibling("flying_enemy");
		}
		if (entity_iterator->data->name == "ground_enemy")
		{
			ret = entity_iterator->data->Load(genemy_data);
			genemy_data = genemy_data.next_sibling("ground_enemy");
		}
		entity_iterator = entity_iterator->next;
	}

	return ret;
}

bool j1EntityManager::Save(pugi::xml_node& data) const
{
	bool ret = true;

	p2List_item<Entity*>* entity_iterator;
	entity_iterator = entity_list.start;

	while (entity_iterator != NULL && ret == true)
	{
		ret = entity_iterator->data->Save(data);
		entity_iterator = entity_iterator->next;
	}

	return ret;
}

//Calls all entities' SetToStart Function
void j1EntityManager::SetToStart()
{
	p2List_item<Entity*>* entity_iterator;
	entity_iterator = entity_list.start;

	while (entity_iterator != NULL)
	{
		entity_iterator->data->SetToStart();
		entity_iterator = entity_iterator->next;
	}

}