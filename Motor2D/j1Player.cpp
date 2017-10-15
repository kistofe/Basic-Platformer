#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1Render.h"
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
	walking.speed = 0.3f;

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
	running.speed = 0.4f,

	//jumping animation
	jump.PushBack({ 0, 138, 54, 69 });
	jump.PushBack({ 54, 138, 54, 69 });
	jump.PushBack({ 108, 138, 54, 69 });
	jump.PushBack({ 162, 138, 54, 69 });
	jump.PushBack({ 216, 138, 54, 69 });
	jump.PushBack({ 270, 138, 54, 69 });
	jump.PushBack({ 324, 138, 54, 69 });
	jump.PushBack({ 378, 138, 54, 69 });
	jump.loop = false;
	jump.speed = 0.4f;

}


j1Player::~j1Player()
{
}

bool j1Player::Awake()
{
	
	return true;
}

bool j1Player::Start()
{
	LOG("Loading player");

	graphics = App->tex->Load("images/Ramona.png");

	player_pos.create(App->map->data.object.start->data->x, App->map->data.object.start->data->y);

	player_collider = App->collision->AddCollider({ player_pos.x, player_pos.y, 40, 65 }, COLLIDER_PLAYER);

	future_player_col = App->collision->AddCollider({ player_collider->rect.x, player_collider->rect.y, 40, 65 }, COLLIDER_FPLAYER);

	return true;
}

bool j1Player::PreUpdate()
{
	player_speed.x = original_speed.x;
	player_speed.y = original_speed.y;
	
	return true;
}


bool j1Player::Update(float dt) /* Dont add more parameters or update wont be called */
{

	SetSpeed();

	player_pos.x += player_speed.x;
	player_pos.y += player_speed.y;


	current_animation = &idle;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		facing_right = true;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		facing_right = false;
	if (!is_jumping)

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		current_animation = &walking;

	if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
		current_animation = &running;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		current_animation = &idle;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		current_animation = &jump;
		
	if (is_jumping)
		current_animation = &jump;
	

	if (facing_right)
		App->render->Blit(graphics, player_pos.x, player_pos.y, &(current_animation->GetCurrentFrame()));

	if (!facing_right)
		App->render->Blit(graphics, player_pos.x, player_pos.y, &(current_animation->GetCurrentFrame()), 1.0F, 0.0, 2147483647, 2147483647, true);

	player_collider->SetPos(player_pos.x + 7, player_pos.y + 4);
	future_player_col->SetPos(player_pos.x + 7 + player_speed.x, player_pos.y + 4 + player_speed.y);

	return true;
}

bool j1Player::CleanUp()
{
	LOG("Unloading player");
	
	App->tex->UnLoad(graphics);
	
		return true;
}

//Load Player info
bool j1Player::Load(pugi::xml_node& data)
{
	player_pos.x = data.child("position").attribute("x").as_int();
	player_pos.y = data.child("position").attribute("y").as_int();

	player_speed.y = data.child("velocity").attribute("y").as_float();
	
	return true;
}

//Save Player info
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
	original_speed.x = 0;
	//original_speed.y = App->scene->gravity;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
	{
		player_speed.x = 3.0f;
		facing_right = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
	{
		player_speed.x = -3.0f;
		facing_right = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
		player_speed.x = player_speed.x * 1.75f;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		player_speed.y = -12.0f;

		is_jumping = true;
	}
		
	return;
}
