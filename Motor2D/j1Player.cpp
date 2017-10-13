#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Map.h"


j1Player::j1Player()
{
	name.create("player");
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

bool j1Player::Awake()
{
	/*for (int i = 0; i < App->map->data.obj_layers.count(); i++)
	{
		// Loop for finding the object group where the player is, probably doesnt work
	}*/
	
	return true;
}

bool j1Player::Start()
{
	LOG("Loading player");

	graphics = App->tex->Load("images/Ramona.png");

	player_pos.create(App->map->data.object.start->data->x, App->map->data.object.start->data->y);
 // Hardcoded until object reading works, replace with player_pos.create(read_x, read_y) or smth

	return true;
}

bool j1Player::PreUpdate()
{
	return true;
}

bool j1Player::Update(float dt) // Dont add more parameters or update wont be called)
{
	SetSpeed();

	current_animation = &idle;

	player_pos.x += player_speed.x;
	player_pos.y += player_speed.y;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		LOG("Player position: x = %i, y = %i", player_pos.x, player_pos.y );
		LOG("Speed.x = %f", player_speed.x);
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		facing_right = true;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		facing_right = false;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		current_animation = &walking;

	if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
		current_animation = &running;

	if (facing_right)
		App->render->Blit(graphics, player_pos.x, player_pos.y, &(current_animation->GetCurrentFrame()));

	if (!facing_right)
		App->render->Blit(graphics, player_pos.x, player_pos.y, &(current_animation->GetCurrentFrame()), 1.0F, 0.0, 2147483647, 2147483647, true);


	return true;
}

bool j1Player::CleanUp()
{
	LOG("Unloading player");
	
	App->tex->UnLoad(graphics);
	
		return true;
}

//Load Player Pos and y speed
bool j1Player::Load(pugi::xml_node& data)
{
	player_pos.x = data.child("position").attribute("x").as_int();
	player_pos.y = data.child("position").attribute("y").as_int();

	player_speed.y = data.child("velocity").attribute("y").as_float();

	return true;
}

bool j1Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node pos = data.append_child("position");

	pos.append_attribute("x") = player_pos.x;
	pos.append_attribute("y") = player_pos.y;

	pugi::xml_node vel = data.append_child("velocity");

	vel.append_attribute("y") = player_speed.y;

	return true;
}

void j1Player::SetSpeed()
{
	player_speed.x = 0;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
	{
		player_speed.x = 2.5f;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
	{
		player_speed.x = -2.5f;
	}
	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
		player_speed.x = player_speed.x * 1.75;

	return;
}
