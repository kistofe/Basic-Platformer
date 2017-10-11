#include "j1Player.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"


j1Player::j1Player()
{
	graphics = NULL;
	current_animation = NULL;

	//idle animation
	idle->PushBack({ 0, 0, 50, 69 });

	//walking animation
	walking->PushBack({ 0, 70, 50, 69 });
	walking->PushBack({ 60, 70, 50, 69 });
	walking->PushBack({ 120, 70, 50, 69 });
	walking->PushBack({ 180, 70, 50, 69 });
	walking->PushBack({ 240, 70, 50, 69 });
	walking->PushBack({ 300, 70, 50, 69 });
	walking->loop = true;
	walking->speed = 1.0f;

	//running animation
	running->PushBack({ 0, 210, 50, 69 });
	running->PushBack({ 60, 210, 50, 69 });
	running->PushBack({ 120, 210, 50, 69 });
	running->PushBack({ 180, 210, 50, 69 });
	running->PushBack({ 180, 210, 50, 69 });
	running->PushBack({ 240, 210, 50, 69 });
	running->PushBack({ 300, 210, 50, 69 });
	running->PushBack({ 360, 210, 50, 69 });
	running->loop = true;
	running->speed = 1.5f,

	//jumping animation
	jump->PushBack({ 0, 140, 50, 69 });
	jump->PushBack({ 60, 140, 50, 69 });
	jump->PushBack({ 120, 140, 50, 69 });
	jump->PushBack({ 180, 140, 50, 69 });
	jump->PushBack({ 180, 140, 50, 69 });
	jump->PushBack({ 240, 140, 50, 69 });
	jump->PushBack({ 300, 140, 50, 69 });
	jump->PushBack({ 360, 140, 50, 69 });
	jump->loop = false;
	jump->speed = 1.5f;

}


j1Player::~j1Player()
{
}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player");

	graphics = App->tex->Load("Motor2D/Game/images/Ramona.png");

	return true;
}

bool j1Player::CleanUp()
{
	LOG("Unloading player");
	
	App->tex->UnLoad(graphics);
	
		return true;
}


