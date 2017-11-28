#include <math.h>
#include "j1App.h"
#include "j1SceneSwitch.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "Player.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "p2Log.h"

#include "Brofiler\Brofiler.h"

j1SceneSwitch::j1SceneSwitch()
{
	name.create("scene_switch");
	screen = { 0, 0, 1920, 1080};
}

j1SceneSwitch::~j1SceneSwitch()
{}

bool j1SceneSwitch::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool j1SceneSwitch::Update(float d_time)
{
	BROFILER_CATEGORY("j1SceneSwitch - Update", Profiler::Color::CadetBlue);
	
	return true;
}

bool j1SceneSwitch::CleanUp()
{
	return false;
}

bool j1SceneSwitch::SwitchMap(const char* map_on)
{
	//Unload current screen
	App->map->CleanUp();

	//Load Again
	App->scene->Initialize(map_on);

	return true;
}

bool j1SceneSwitch::DestroyEnemies()
{
	return false;
}
