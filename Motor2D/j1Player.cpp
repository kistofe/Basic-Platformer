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

	//walking animation

	//running animation

	//jumping animation

	//etc
}


j1Player::~j1Player()
{
}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player");

	graphics = App->tex->Load(/*ramona flowers spritsheet*/);

	return true;
}

bool j1Player::CleanUp()
{
	LOG("Unloading player");
	
	App->tex->UnLoad(graphics);
	
		return true;
}


