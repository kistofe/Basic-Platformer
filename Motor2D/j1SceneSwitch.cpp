#include <math.h>
#include "j1App.h"
#include "j1SceneSwitch.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1InGameScene.h"
#include "Player.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1EntityManager.h"
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
	return true;
}

bool j1SceneSwitch::SwitchMap(const char* map_on)
{
	//Unload current screen
	App->collision->CleanUp();
	App->entities->CleanUp();
	App->map->CleanUp();

	//Load Again
	App->map->Start();
	App->map->active = true;
	App->ingamescene->InitializeMap(map_on);
	App->entities->Start();
	App->entities->active = true;
	App->collision->Start();
	App->collision->active = true;

	return true;
}

bool j1SceneSwitch::DestroyEnemies()
{
	return false;
}
