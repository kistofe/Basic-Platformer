#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "Player.h"
#include "j1Scene.h"
#include "j1SceneSwitch.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Pathfinding.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Audio.h"

Player::Player(uint x, uint y) : Entity(Entity::EntityType::PLAYER)
{
	name.create("player");
	CreateAnimationPushBacks();

	pugi::xml_document config_file;
	pugi::xml_node config;
	pugi::xml_node data;

	config = App->LoadConfig(config_file);
	data = config.child("entity_manager").child("player");

	position.x = x;
	position.y = y;

	moving_speed = data.child("moving_speed").attribute("value").as_float();
	jumping_speed = data.child("jumping_speed").attribute("value").as_float();
	collider_offset.x = data.child("collider_offset_x").attribute("value").as_int();
	collider_offset.y = data.child("collider_offset_y").attribute("value").as_int();
	death_sfx_source = data.child("death_sfx").attribute("source").as_string();
	jumping_sfx_source = data.child("jump_sfx").attribute("source").as_string();
	landing_sfx_source = data.child("land_sfx").attribute("source").as_string();
}

Player::~Player()
{
}


bool Player::Start()
{
	LOG("Loading player");
	
	texture = App->tex->Load("images/Ramona.png");
	
	//Creating Colliders
	collider = App->collision->AddCollider({ position.x + collider_offset.x, position.y + collider_offset.y, 35, 65 }, COLLIDER_PLAYER, this);
	future_collider = App->collision->AddCollider({ collider->rect.x, collider->rect.y, 35, 65 }, COLLIDER_FPLAYER, this);

	current_animation = &idle;


	return true;
}

bool Player::PreUpdate(float d_time)
{
	SetSpeed(d_time);

	//Update Future Player Collider with new speed
	future_collider->SetPos((collider->rect.x + speed.x), (collider->rect.y + speed.y));

	return true;
}


bool Player::Update(float d_time) 
{
	SetCameraToPlayer();
	//Check Horizontal Movement ----------------------------------------
	//Right
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		facing_right = true;
	
	//Left
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		facing_right = false;
	//--------------------------------------------------------------------

	//Check Jump ---------------------------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jumps_left != 0)
	{
		jumps_left--;
		is_grounded = false;
		App->audio->PlayFx(jumping_sfx, 0, App->audio->music_vol);
	}

	//Set Animation ------------------------------------------------------
	SetAnimations();
	//Update Player Position ---------------------------------------------	
	Move();
	//Update Player Collider ---------------------------------------------
	collider->SetPos((position.x + collider_offset.x), (position.y + collider_offset.y));
	//Update Player's Blit ----------------------------------------------
	Draw();

	// PATHFINDING DEBUG	 --------------------------------------------

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		LOG("Current Player Position: %i, %i", App->map->WorldToMap(position.x, position.y));

		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint mousepos = App->render->ScreenToWorld(x, y);
		mousepos = App->map->WorldToMap(mousepos.x, mousepos.y);

		LOG("Clicked Position: %i, %i", mousepos.x, mousepos.y);

		iPoint next_tile = App->pathfinding->GetNextTile(mousepos, App->map->WorldToMap(position.x, position.y));
		LOG("Next tile: %i, %i", next_tile.x, next_tile.y);

	}


	//--------------------------------------------------------------------

	return true;
}

bool Player::CleanUp()
{
	LOG("Unloading player");
	
	App->tex->UnLoad(texture);
	
		return true;
}

//Load Player info
bool Player::Load(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
	speed.x = data.child("velocity").attribute("x").as_float();
	speed.y = data.child("velocity").attribute("y").as_float();
	is_grounded = data.child("status").child("is_grounded").attribute("value").as_bool();
	facing_right = data.child("status").child("facing_right").attribute("value").as_bool();
	jumps_left = data.child("status").child("jumps_left").attribute("value").as_uint();
	
	return true;
}

//Save Player info
bool Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node player = data.append_child("player");
	pugi::xml_node pos = player.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;

	pugi::xml_node vel = player.append_child("velocity");

	vel.append_attribute("x") = speed.x;
	vel.append_attribute("y") = speed.y;

	pugi::xml_node status = player.append_child("status");

	status.append_child("is_grounded").append_attribute("value") = is_grounded;
	status.append_child("facing_right").append_attribute("value") = facing_right;
	status.append_child("jumps_left").append_attribute("value") = jumps_left;

	return true;
}

void Player::CreateAnimationPushBacks()
{
	current_animation = NULL;

	//idle animation
	idle.PushBack({ 0, 0, 54, 69 });
	
	//running animation
	run.PushBack({ 0, 207, 54, 69 });
	run.PushBack({ 54, 207, 54, 69 });
	run.PushBack({ 108, 207, 54, 69 });
	run.PushBack({ 162, 207, 54, 69 });
	run.PushBack({ 216, 207, 54, 69 });
	run.PushBack({ 270, 207, 54, 69 });
	run.PushBack({ 324, 207, 54, 69 });
	run.PushBack({ 378, 207, 54, 69 });
	run.loop = true;
	run.speed = 0.4f;
	
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
	jump.speed = 0.5f;

	//Double Jump animation
	double_jump.PushBack({ 0, 276, 54, 69 });
	double_jump.PushBack({ 54, 276, 54, 69 });
	double_jump.PushBack({ 108, 276, 54, 69 });
	double_jump.PushBack({ 162, 276, 54, 69 });
	double_jump.PushBack({ 216, 276, 54, 69 });
	double_jump.loop = true;
	double_jump.speed = 0.4f;

	//Win animation
	win.PushBack({  0,  490, 54, 69});
	win.PushBack({ 60,  490, 54, 69});
	win.PushBack({ 120, 490, 54, 69 });
	win.PushBack({ 180, 490, 54, 69 });
	win.PushBack({ 240, 490, 54, 69 });
	win.loop = false;
	win.speed = 0.3f;

	//Falling animation
	fall.PushBack({ 378, 138, 54, 69 });

	//Damaged animation
	damaged.PushBack({ 0, 420, 54, 69 });
	damaged.PushBack({ 60, 420, 54, 69 });
	damaged.loop = false;
	damaged.speed = 0.2f;

}


void Player::SetCameraToPlayer()
{
	App->render->camera.x = App->render->camera.w / 3 - position.x;
	if (App->render->camera.x > 0)
		App->render->camera.x = 0;
	else if (App->render->camera.x <= App->map->max_map_x)
		App->render->camera.x = App->map->max_map_x;
	App->render->camera.y = App->render->camera.h / 1.35 - position.y;
	if (App->render->camera.y > 0)
		App->render->camera.y = 0;
	else if (App->render->camera.y <= App->map->max_map_y)
		App->render->camera.y = App->map->max_map_y;
		
}

void Player::SetSpeed(float d_time)
{
	//Set maximum value for gravity
	if (speed.y < App->scene->max_gravity.y)
		speed.y -= App->scene->gravity.y * d_time;

	else
		speed.y = App->scene->max_gravity.y * d_time;
		
	//Set value for Horizontal Speed
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
		speed.x = moving_speed * d_time;

	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
		speed.x = -moving_speed * d_time;
		
	else
		speed.x = 0; 
		
	//Set Jumping Speed
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jumps_left != 0)
		speed.y = jumping_speed * d_time;
		
}

void Player::SetAnimations()
{
	//Reset animation to idle if no keys are pressed
	current_animation = &idle;
	//Running animation
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		current_animation = &run;
	//Idle when two keys are pressed simultaneously 
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		current_animation = &idle;
	//Jumping animation
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jumps_left == 1) //reset the jump animation each time a regular (not double) jump is performed
		jump.Reset();
	if (jumps_left == 1 && speed.y < 2) //set animation to regular jump if only a jump has been depleted
		current_animation = &jump;
	if (speed.y > 2 && (jumps_left == 2 || jumps_left == 1)) //set animation to fall if the player is still in a regular jump, and is moving downwards, or is falling
		current_animation = &fall;
	if (jumps_left == 0) // set animation to double jump if there's no jumps left
		current_animation = &double_jump;
		
	//Victory animation
	//Death animation
	
}

void Player::OnCollision(Collider * c1, Collider * c2)
{
	if (c1->type == COLLIDER_FPLAYER && c2->type == COLLIDER_WALL)
	{
		SDL_Rect intersect_col;
		if (SDL_IntersectRect(&c1->rect, &c2->rect, &intersect_col));
		//future player collider and a certain collider have collided
		{
			if (speed.y > 0)
			{
				if (collider->rect.y + collider->rect.h <= c2->rect.y)//Checking player is above the collider
				{
					if (speed.x == 0)
						speed.y -= intersect_col.h, jumps_left = 2;

					else if (speed.x < 0)
					{
						if (intersect_col.h >= intersect_col.w)
						{
							if (c1->rect.x <= c2->rect.x + c2->rect.w)
								speed.x += intersect_col.w;
							else
								speed.y -= intersect_col.h, jumps_left = 2;
						}
						else
							speed.y -= intersect_col.h;
					}
					else if (speed.x > 0)
					{
						if (intersect_col.h >= intersect_col.w)
						{
							if (c1->rect.x + c1->rect.w >= c2->rect.x)
								speed.x -= intersect_col.w;
							else
								speed.y -= intersect_col.h, jumps_left = 2;
						}
						else
							speed.y -= intersect_col.h, jumps_left = 2;
					}
				}
				else
				{
					if (speed.x < 0)
						speed.x += intersect_col.w;

					else if (speed.x > 0)
						speed.x -= intersect_col.w;
				}

			}
			else if (speed.y < 0)
			{
				if (collider->rect.y >= c2->rect.y + c2->rect.h)
				{
					if (speed.x == 0)
						speed.y += intersect_col.h;

					else if (speed.x < 0)
					{
						if (intersect_col.h >= intersect_col.w)
						{
							if (c1->rect.x <= c2->rect.x + c2->rect.w)
								speed.x += intersect_col.w;
							else
								speed.y += intersect_col.h;
						}
						else
							speed.y += intersect_col.h;
					}
					else if (speed.x > 0)
					{
						if (intersect_col.h >= intersect_col.w)
						{
							if (c1->rect.x + c1->rect.w >= c2->rect.x)
								speed.x -= intersect_col.w;
							else
								speed.y += intersect_col.h;
						}
						else
							speed.y += intersect_col.h;
					}

				}
				
				else
				{
					if (speed.x < 0)
						speed.x += intersect_col.w;

					else if (speed.x > 0)
						speed.x -= intersect_col.w;
				}
			}
			
			else
			{
				if (speed.x < 0)
					speed.x += intersect_col.w;

				else if (speed.x > 0)
					speed.x -= intersect_col.w;
			}
		
		}

		//Rounding off speed value
		if (speed.y < 1 && speed.y > -1)
			speed.y = 0;
		if (speed.x < 1 && speed.x > -1)
			speed.x = 0;
	}


	if (c1->type == COLLIDER_FPLAYER && c2->type == COLLIDER_ENDOFLEVEL)
		App->sceneswitch->FadeToBlack();
	

	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_DEATH)
	{
		App->audio->PlayFx(death_sfx, 0, App->audio->music_vol);
		SetToStart();
	}

}


