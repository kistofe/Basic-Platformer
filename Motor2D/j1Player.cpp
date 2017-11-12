#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1SceneSwitch.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Audio.h"

j1Player::j1Player()
{
	name.create("player");
	
	CreateAnimationPushBacks();
}


j1Player::~j1Player()
{
}

bool j1Player::Awake(pugi::xml_node& data)
{

	//Reading Collider offsets
	collider_offset.x = data.child("collider_offset_x").attribute("value").as_int();
	collider_offset.y = data.child("collider_offset_y").attribute("value").as_int();
	
	//Reading player velocities
	moving_speed = data.child("moving_speed").attribute("value").as_float();
	jumping_speed = data.child("jumping_speed").attribute("value").as_float();

	//Reading player sfx source
	jumping_sfx_source = data.child("jump_sfx").attribute("source").as_string();
	landing_sfx_source = data.child("landing_sfx").attribute("source").as_string();
	death_sfx_source = data.child("death_sfx").attribute("source").as_string();

	return true;
}

bool j1Player::Start()
{
	LOG("Loading player");

	player_tex = App->tex->Load("images/Ramona.png");
	
	//Reading Player initial position from Object layer
	position.create(App->map->data.object.start->data->x, App->map->data.object.start->data->y);

	//Creating Colliders
	collider = App->collision->AddCollider({ position.x + collider_offset.x, position.y + collider_offset.y, 35, 65 }, COLLIDER_PLAYER, this);
	future_collider = App->collision->AddCollider({ collider->rect.x, collider->rect.y, 35, 65 }, COLLIDER_FPLAYER, this);

	//Loading Player's Sfx
	jumping_sfx = App->audio->LoadFx(jumping_sfx_source.GetString());
	landing_sfx = App->audio->LoadFx(landing_sfx_source.GetString());
	death_sfx = App->audio->LoadFx(death_sfx_source.GetString());

	current_animation = &idle;


	return true;
}

bool j1Player::PreUpdate()
{
	SetSpeed();

	//Update Future Player Collider with new speed
	future_collider->SetPos((collider->rect.x + speed.x), (collider->rect.y + speed.y));

	return true;
}


bool j1Player::Update(float d_time) 
{

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

	//Update Player Flip -------------------------------------------------
	if (facing_right)
		App->render->Blit(player_tex, position.x, position.y, &(current_animation->GetCurrentFrame()));

	if (!facing_right)
		App->render->Blit(player_tex, position.x, position.y, &(current_animation->GetCurrentFrame()), 1.0F, 0.0, 2147483647, 2147483647, true);
	
	//Update Player Position ---------------------------------------------
	position.x += speed.x;
	position.y += speed.y;

	//Update Player Collider ---------------------------------------------
	collider->SetPos((position.x + collider_offset.x),( position.y + collider_offset.y));

	return true;
}

bool j1Player::CleanUp()
{
	LOG("Unloading player");
	
	App->tex->UnLoad(player_tex);
	
		return true;
}

//Load Player info
bool j1Player::Load(pugi::xml_node& data)
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
bool j1Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node pos = data.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;

	pugi::xml_node vel = data.append_child("velocity");

	vel.append_attribute("x") = speed.x;
	vel.append_attribute("y") = speed.y;

	pugi::xml_node status = data.append_child("status");

	status.append_child("is_grounded").append_attribute("value") = is_grounded;
	status.append_child("facing_right").append_attribute("value") = facing_right;
	status.append_child("jumps_left").append_attribute("value") = jumps_left;

	return true;
}

void j1Player::CreateAnimationPushBacks()
{
	player_tex = NULL;
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
	run.speed = 0.4f,
	
	//jumping animation
	jump.PushBack({ 0, 138, 54, 69 });
	jump.PushBack({ 54, 138, 54, 69 });
	jump.PushBack({ 108, 138, 54, 69 });
	jump.PushBack({ 162, 138, 54, 69 });
	jump.PushBack({ 216, 138, 54, 69 });
	jump.PushBack({ 270, 138, 54, 69 });
	jump.PushBack({ 324, 138, 54, 69 });
	jump.PushBack({ 378, 138, 54, 69 });
	jump.PushBack({ 437, 138, 54, 69 });
	jump.PushBack({ 0, 0, 54, 69 });
	jump.loop = false;
	jump.speed = 0.4f;

	//Double Jump animation
	double_jump.PushBack({ 0, 280, 54, 69 });
	double_jump.PushBack({ 0, 280, 54, 69 });
	double_jump.PushBack({ 0, 280, 54, 69 });
	double_jump.PushBack({ 0, 280, 54, 69 });
	double_jump.PushBack({ 0, 280, 54, 69 });
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
	falling.PushBack({ 378, 138, 54, 69 });

	//Damaged animation
	damaged.PushBack({ 0, 420, 54, 69 });
	damaged.PushBack({ 60, 420, 54, 69 });
	damaged.loop = false;
	damaged.speed = 0.2f;

}

void j1Player::SetSpeed()
{
	//Set maximum value for gravity
	if (speed.y < App->scene->max_gravity.y)
		speed.y -= App->scene->gravity.y;

	else
		speed.y = App->scene->max_gravity.y;
		
	//Set value for Horizontal Speed
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
		speed.x = moving_speed;

	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
		speed.x = -moving_speed;
		
	else
		speed.x = 0; 
		
	//Set Jumping Speed
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jumps_left != 0)
		speed.y = jumping_speed;
		
}

void j1Player::SetAnimations()
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
	if (jumps_left == 1)
		current_animation = &jump;
	if (jumps_left == 0)
		current_animation = &double_jump;
		
	//Victory animation
	//Death animation
	//Falling animation
	//Idle animation
	
}

void j1Player::OnCollision(Collider * c1, Collider * c2)
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
								speed.y -= intersect_col.h;
						}
						else
							speed.y -= intersect_col.h, jumps_left = 2;
					}
					else if (speed.x > 0)
					{
						if (intersect_col.h >= intersect_col.w)
						{
							if (c1->rect.x + c1->rect.w >= c2->rect.x)
								speed.x -= intersect_col.w;
							else
								speed.y -= intersect_col.h;
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
						speed.y += intersect_col.h, jumps_left = 2;

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
							speed.y += intersect_col.h, jumps_left = 2;
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
							speed.y += intersect_col.h, jumps_left = 2;
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
	}


	if (c1->type == COLLIDER_FPLAYER && c2->type == COLLIDER_ENDOFLEVEL)
		App->sceneswitch->FadeToBlack();
	

	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_DEATH)
	{
		App->audio->PlayFx(death_sfx, 0, App->audio->music_vol);
		SetToStart();
	}

}

void j1Player::SetToStart()
{//Loop should be done maybe?
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->sceneswitch->FadeToBlack();

	position.x = App->map->data.object.start->data->x;
	position.y = App->map->data.object.start->data->y;
	speed.x = 0;
	speed.y = 0;
	current_animation = &idle;
}

