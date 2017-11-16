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
	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			if (App->scene->current_lvl == 1) //&& App->input->GetKey(!SDL_SCANCODE_F1) == KEY_DOWN)
			{
				SwitchMap("Level2.tmx");
				App->scene->NextLevel();
				App->audio->PlayMusic("audio/music/Level_2.ogg");
			}

			else if (App->scene->current_lvl == 2)
			{
				SwitchMap("Level1finalinprogress.tmx");
				App->scene->NextLevel();
				App->audio->PlayMusic("audio/music/Level_1.ogg");
			}
			//App->player->SetToStart(); 
			total_time += total_time;
			start_time = SDL_GetTicks();
			fading = false;
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time)
		{
			current_step = fade_step::none;
		}


	} break;
	}

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);
	return true;
}

bool j1SceneSwitch::CleanUp()
{
	return false;
}

bool j1SceneSwitch::FadeToBlack(float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (uint)(time * 0.5f * 1000.0f);
		fading = true;
		ret = true;
	}

	return ret;
	return false;
}

bool j1SceneSwitch::SwitchMap(const char* map_on)
{
	//Unload current screen
	App->map->CleanUp();

	//Load Again
	App->map->Load(map_on);
	return true;
}

bool j1SceneSwitch::IsFading()
{
	return current_step != fade_step::none;
}
