#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "Player.h"
#include "j1Scene.h"
#include "j1SceneSwitch.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"
#include "j1EntityManager.h"
#include "j1GuiController.h"

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

	gravity.x = 0;
	gravity.y = 0;
	max_gravity.x = 0;
	max_gravity.y = 0;
	
	return ret;
}


// Called before the first frame
bool j1Scene::Start()
{
	
	Initialize(App->map->map_name.start->data->GetString());

	App->audio->PlayMusic("audio/music/Level_1.ogg");
	AddUiElems();

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float d_time)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->entities->SetToStart();
		
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		App->entities->SetToStart();

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		//App->sceneswitch->SwitchMap("Level2inprogress.tmx");
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		if (App->entities->player1->god_mode)
			App->entities->player1->god_mode = false;
		else
			App->entities->player1->god_mode = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
		App->audio->VolumeControl();

	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
		App->audio->VolumeControl();

	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(),
		map_coordinates.x, map_coordinates.y);

	
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

void j1Scene::Initialize(const char* map_initialized)
{
	App->map->Load(map_initialized);
	App->map->SetEntities();


	App->entities->SetToStart();
	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);

	RELEASE_ARRAY(data);
}

bool j1Scene::OnEvent(Widget * ui_elem, int event)
{
	/*if (ui_elem->type == BUTTON)
		switch (event)
		{
		case MOUSE_CLICK:
		}
		*/
	
	return true;
}

void j1Scene::AddUiElems()
{
	my_text = (Label*)App->gui->CreateWidget(UiElemType::LABEL, 200, 1500, this);
	my_text->SetText("HELLO", { (255), (255), (255), (255) });

	test_button = (Button*)App->gui->CreateWidget(UiElemType::BUTTON, 200, 1450, this);
	test_button->SetSection({ 10, 8, 192, 64 }, { 10, 72, 192, 64 }, { 10, 136, 192, 64 });
	test_button_label = (Label*)App->gui->CreateWidget(UiElemType::LABEL, 225, 1525, this);
	test_button->CreateButtonLabel(test_button_label, "HELLO", { 255,255,255,255 });

	UIWindow* ui_window = (UIWindow*)App->gui->CreateWidget(UiElemType::WINDOW, 200, 1000, this);
}
