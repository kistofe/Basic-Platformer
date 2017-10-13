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

	//MapData* map_set = new MapData;

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
	{
		App->player->player_pos.x = App->map->data.object.start->data->x;//When the second map is loaded successfully, this method will need to be revised and probably added to a function
		App->player->player_pos.y = App->map->data.object.start->data->y;
		App->player->facing_right = true;
	}

	//if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		
	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		App->map->MapSwitch("test2.tmx");
	}

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += 1;

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
