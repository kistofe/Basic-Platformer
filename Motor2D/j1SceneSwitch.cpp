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
	bool ret = true;

	if (current_step == fade_step::none)
		return ret;

	float normalized = MIN(1.0f, switchtimer.ReadSec() / fadetime);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (switchtimer.ReadSec() >= fadetime)
		{
			to_disable->Disable();
			App->gui->CleanUp();
			App->gui->Start();
			switchtimer.Start();
			current_step = fade_step::fade_from_black;
			to_enable->Enable();
		}
	}break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;

		if (switchtimer.ReadSec() >= fadetime)
		{
			current_step = fade_step::none;
		}
	}break;
	}

	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return ret;
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
	App->ingamescene->NextLevel();

	return true;
}

bool j1SceneSwitch::DestroyEnemies()
{
	return false;
}

bool j1SceneSwitch::SwitchScene(j1Module * SceneIn, j1Module * SceneOut)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		switchtimer.Start();
		to_enable = SceneIn;
		to_disable = SceneOut;
		ret = true;
	}
	return ret;
}

bool j1SceneSwitch::IsSwitching() const
{
	return (current_step != fade_step::none);
}
