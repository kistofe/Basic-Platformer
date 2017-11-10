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


j1Player::j1Player()
{
	name.create("player");
	
	CreateAnimationPushBacks();
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

	player_tex = App->tex->Load("images/Ramona.png");

	position.create(App->map->data.object.start->data->x, App->map->data.object.start->data->y);

	collider = App->collision->AddCollider({ position.x + collider_offset.x, position.y + collider_offset.y, 35, 65 }, COLLIDER_PLAYER, this);

	future_collider = App->collision->AddCollider({ collider->rect.x, collider->rect.y, 35, 65 }, COLLIDER_FPLAYER, this);

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
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		is_jumping = true; is_grounded = false;
		
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

	speed.y = data.child("velocity").attribute("y").as_float();
	
	return true;
}

//Save Player info
bool j1Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node pos = data.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;

	pugi::xml_node vel = data.append_child("velocity");

	vel.append_attribute("y") = speed.y;

	return true;
}

void j1Player::CreateAnimationPushBacks()
{
	player_tex = NULL;
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
		speed.x = 3.0f;

	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
		speed.x = -3.0f;
		
	else
		speed.x = 0;

	// Set Running speed
	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
		speed.x = speed.x * running_accel;
	
	//Set Jumping Speed
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		speed.y = -14.5f;
	
}

void j1Player::SetAnimations()
{
	//Walking animation
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		current_animation = &walking;
	//Running animation
	if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
		current_animation = &running;
	//Idle when two keys are pressed simultaneously 
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		current_animation = &idle;
	//Jumping animation
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		//Single jump
		if (is_jumping)
			current_animation = &jump;
		
		//	else //Double jump
		//	current_animation = &double_jump;		
	}
	
	
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
				if (speed.x == 0 && c1->rect.x + c1->rect.w > c2->rect.x && c1->rect.x + c1->rect.w < c2->rect.x + c2->rect.w)//player is not moving
					speed.y -= intersect_col.h,	is_jumping = false;
				
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
						speed.y -= intersect_col.h;
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
						speed.y -= intersect_col.h;
				}
			}

			else if (speed.y < 0)
			{
				if (speed.x == 0 && c1->rect.x + c1->rect.w > c2->rect.x && c1->rect.x + c1->rect.w < c2->rect.x + c2->rect.w)//player is not moving
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
	}


	if (c1->type == COLLIDER_FPLAYER && c2->type == COLLIDER_ENDOFLEVEL)
	{
		App->sceneswitch->FadeToBlack();
	}

}

