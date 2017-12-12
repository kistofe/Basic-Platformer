#include "j1App.h"
#include "j1Collision.h"
#include "Coin.h"
#include "j1Textures.h"



Coin::Coin(uint x, uint y) : Entity(Entity::EntityType::COIN)
{
	name.create("coin");
	CreateAnimationPushBacks();

	pugi::xml_document config_file;
	pugi::xml_node config;
	pugi::xml_node data;

	config = App->LoadConfig(config_file);
	data = config.child("entity_manager").child("coin");

	default_animation = &spin;

	coin_tex = App->tex->Load("images/coin.png");
	current_tex = coin_tex;
	position.x = x;
	position.y = y;

	original_position.x = x;
	original_position.y = y;

	collider_offset.x = data.child("collider_offset_x").attribute("value").as_int();
	collider_offset.y = data.child("collider_offset_y").attribute("value").as_int();

}


Coin::~Coin()
{
}

bool Coin::Start()
{

	collider = App->collision->AddCollider({ position.x + collider_offset.x, position.y + collider_offset.y, 48, 48 }, COLLIDER_COIN, this);

	current_animation = &spin;

	return true;
}

bool Coin::Update()
{
	return true;
}

bool Coin::CleanUp()
{
	App->tex->UnLoad(coin_tex);
	return true;
}

bool Coin::Load(pugi::xml_node&)
{
	return true;
}

bool Coin::Save(pugi::xml_node&) const
{
	return true;
}

void Coin::CreateAnimationPushBacks()
{
	current_animation = NULL;

	spin.PushBack({ 2, 26, 48, 48 });
	spin.PushBack({ 62, 26, 48, 48 });
	spin.PushBack({ 122, 26, 48, 48 });
	spin.PushBack({ 182, 26, 48, 48 });
	spin.PushBack({ 242, 26, 48, 48 });
	spin.PushBack({ 302, 26, 48, 48 });
	spin.loop = true;
	spin.speed = 0.4f;
}
