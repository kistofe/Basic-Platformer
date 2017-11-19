#include "Ground_Enemy.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Map.h"


Ground_Enemy::Ground_Enemy(uint x, uint y) : Enemy(Entity::EntityType::GROUND_ENEMY)
{
	name.create("ground_enemy");
	CreateAnimationPushBacks();

	position.x = x;
	position.y = y;


}


Ground_Enemy::~Ground_Enemy()
{
}

bool Ground_Enemy::Start()
{
	LOG("Loading Ground Enemy");

	texture = App->tex->Load("images/Ground Enemy.png");

	//Creating Colliders
	collider = App->collision->AddCollider({ position.x + collider_offset.x, position.y + collider_offset.y, 30, 30 }, COLLIDER_ENEMY, this);
	future_collider = App->collision->AddCollider({ collider->rect.x, collider->rect.y, 30, 30 }, COLLIDER_FUTURE, this);

	current_animation = &idle;

	return true;
}

bool Ground_Enemy::PreUpdate(float d_time)
{
	//Call ChangeSpeed()
	//Update Future collider
	future_collider->SetPos((collider->rect.x + speed.x), (collider->rect.y + speed.y));
	facing_right = false;
	return true;
}

bool Ground_Enemy::Update(float d_time)
{
	//Check bool alive (only enter if the enemy is still alive
	//Call Draw()
	//Call Move()
	//Call SetAnimations()
	//Update Collider Position-------------------------------------
	collider->SetPos(position.x + collider_offset.x, position.y + collider_offset.y);
	//Update Blit -------------------------------------------------
	Draw();
	return true;
}

bool Ground_Enemy::CleanUp()
{
	//Unload Texture
	App->tex->UnLoad(texture);
	return true;
}

bool Ground_Enemy::Load(pugi::xml_node &)
{
	//Load position
	//Load velocity
	//Load status (dead or alive)
	return true;
}

bool Ground_Enemy::Save(pugi::xml_node &) const
{
	//Save position
	//Save velocity
	//Save status
	return true;
}

void Ground_Enemy::OnCollision(Collider * c1, Collider * c2)
{
	//Check collision with scenario
	//Check collision with player
}


void Ground_Enemy::SetAnimations()
{
	//Check case and adjust animation
}

void Ground_Enemy::CreateAnimationPushBacks()
{
	current_animation = NULL;

	idle.PushBack({ 0,0,96, 52 });
	idle.PushBack({ 96,0,96, 52 });
	idle.PushBack({ 192,0,96, 52 });
	idle.PushBack({ 288,0,96, 52 });
	idle.loop = true;
	idle.speed = 0.2f;
}


