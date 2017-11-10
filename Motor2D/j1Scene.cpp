#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1SceneSwitch.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene"); 
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	//Loop to load all maps
	for (pugi::xml_node map = config.child("map_name"); map && ret; map = map.next_sibling("map_name"))
	{
		p2SString* map_set = new p2SString;
		
		if (ret == true)
		{
			ret = LoadMapAtrib(map, map_set);
		}
		map_name.add(map_set);
	}
	
	return ret;
}

bool j1Scene::LoadMapAtrib(pugi::xml_node& config, p2SString* map_set)
{
	bool ret = true;

	map_set->create(config.attribute("name").as_string());

	return ret;
}


// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load(map_name.start->data->GetString());

	App->audio->PlayMusic("audio/music/Level_1.ogg");

	gravity.x = 0;
	gravity.y = -1;

	max_gravity.x = 0;
	max_gravity.y = 10;
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->map->LoadBeginning();
		
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		App->map->LoadBeginning();

	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		App->sceneswitch->FadeToBlack();
	
	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
		App->audio->VolumeControl();

	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
		App->audio->VolumeControl();

	//App->render->Blit(img, 0, 0);
	App->map->Draw();

	
	// "Map:%dx%d Tiles:%dx%d Tilesets:%d"
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count());

	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	

	return true;
}

void j1Scene::NextLevel()
{
	if (current_lvl != LAST_LVL)
		current_lvl++;

	else if (current_lvl == LAST_LVL)
		current_lvl = 1;
}