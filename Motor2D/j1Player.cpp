#include "j1Player.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Map.h"


j1Player::j1Player()
{
	graphics = NULL;
	current_animation = NULL;

	//idle animation
	idle.PushBack({ 0, 0, 54, 69 });

	//walking animation
	walking.PushBack({ 0, 69, 54, 69 });
	walking.PushBack({ 54, 69, 54, 69 });
	walking.PushBack({ 108, 69, 54, 69 });
	walking.PushBack({ 162, 69, 54, 69 });
	walking.PushBack({ 216, 69, 54, 69 });
	walking.PushBack({ 270, 69, 54, 69 });
	walking.loop = true;
	walking.speed = 0.2f;

	//running animation
	running.PushBack({ 0, 207, 54, 69 });
	running.PushBack({ 54, 207, 54, 69 });
	running.PushBack({ 108, 207, 54, 69 });
	running.PushBack({ 162, 207, 54, 69 });
	running.PushBack({ 216, 207, 54, 69 });
	running.PushBack({ 270, 207, 54, 69 });
	running.PushBack({ 324, 207, 54, 69 });
	running.PushBack({ 378, 207, 54, 69 });
	running.loop = true;
	running.speed = 0.3f,

	//jumping animation
	jump.PushBack({ 0, 140, 54, 69 });
	jump.PushBack({ 60, 140, 54, 69 });
	jump.PushBack({ 120, 140, 54, 69 });
	jump.PushBack({ 180, 140, 54, 69 });
	jump.PushBack({ 180, 140, 54, 69 });
	jump.PushBack({ 240, 140, 54, 69 });
	jump.PushBack({ 300, 140, 54, 69 });
	jump.PushBack({ 360, 140, 54, 69 });
	jump.loop = false;
	jump.speed = 1.5f;

}


j1Player::~j1Player()
{
}

bool j1Player::Awake(pugi::xml_node& object)
{
<<<<<<< HEAD
	
	
		
=======
	player_pos.x = object.attribute("x").as_int();
	player_pos.y = object.attribute("y").as_int();

>>>>>>> origin/master
	return true;
}

bool j1Player::Start(pugi::xml_node& object, ObjLayer* pos)
{
	LOG("Loading player");

<<<<<<< HEAD

	graphics = App->tex->Load("images/Ramona.png");

	pos->x = object.attribute("x").as_int();
	pos->y = object.attribute("y").as_int();
=======
	graphics = App->tex->Load("images/Ramona.png");
>>>>>>> origin/master

	return true;
}

bool j1Player::PreUpdate()
{
	return true;
}

bool j1Player::Update(float dt, ObjLayer* pos)
{
	SetSpeed();

	current_animation = &idle;

	pos->x += player_speed.x;
	pos->y += player_speed.y;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		LOG("Player position: x = %i, y = %i", pos->x, pos->y );
		LOG("Speed.x = %f", player_speed.x);
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		facing_right = true;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		facing_right = false;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		current_animation = &walking;

<<<<<<< HEAD
	App->render->Blit(graphics, pos->x, pos->y, &(current_animation->GetCurrentFrame()));
=======
	if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
		current_animation = &running;

	if (facing_right)
		App->render->Blit(graphics, player_pos.x, player_pos.y, &(current_animation->GetCurrentFrame()));

	if (!facing_right)
		App->render->Blit(graphics, player_pos.x, player_pos.y, &(current_animation->GetCurrentFrame()), 1.0F, 0.0, 2147483647, 2147483647, true);
>>>>>>> origin/master

	return true;
}

bool j1Player::CleanUp()
{
	LOG("Unloading player");
	
	App->tex->UnLoad(graphics);
	
		return true;
}

void j1Player::SetSpeed()
{
	player_speed.x = 0;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
	{
		player_speed.x = 4.0f;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
	{
		player_speed.x = -4.0f;
	}
	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
		player_speed.x = player_speed.x * 1.75;

	return;
}
