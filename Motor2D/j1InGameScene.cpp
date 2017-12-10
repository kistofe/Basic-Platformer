#include "j1App.h"
#include "j1InGameScene.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1SceneSwitch.h"
#include "j1Pathfinding.h"
#include "j1Textures.h"


j1InGameScene::j1InGameScene()
{
	name.create("ingame_scene");
}

j1InGameScene::~j1InGameScene()
{}

bool j1InGameScene::Awake(pugi::xml_node & config)
{
	LOG("Loading InGame Scene");
	bool ret = true;

	gravity.x = config.child("gravity_x").attribute("value").as_float();
	gravity.y = config.child("gravity_y").attribute("value").as_float();
	max_gravity.x = config.child("max_gravity_x").attribute("value").as_float();
	max_gravity.y = config.child("max_gravity_y").attribute("value").as_float();

	char1_life_icon = { 0, 0, 70, 56 };
	char2_life_icon = { 76, 0, 70, 56 };
	time_icon = { 7, 78, 51, 51 };
	return ret;
}

bool j1InGameScene::Start()
{
	InitializeMap(App->map->map_name.start->data->GetString());

	App->audio->PlayMusic("audio/music/Level_1.ogg");
	hud_tex = App->tex->Load("gui/HUD.png");

	AddUiElems();

	return true;
}

bool j1InGameScene::Update(float d_time)
{
	HandleInput();

	App->map->Draw();
	
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(),
		map_coordinates.x, map_coordinates.y);

	UpdateUIpos();
	
	return true;
}

bool j1InGameScene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
		App->sceneswitch->SwitchMap("Level2inprogress.tmx");

	return ret;
}

bool j1InGameScene::CleanUp()
{
	App->tex->UnLoad(hud_tex);

	return true;
}

void j1InGameScene::NextLevel()
{
	if (current_lvl != LAST_LVL)
		current_lvl++;

	else if (current_lvl == LAST_LVL)
		current_lvl = 1;
}

void j1InGameScene::InitializeMap(const char * map_initialized)
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

void j1InGameScene::AddUiElems()
{
	//Current Character Label
	curr_character = (Label*)App->gui->CreateWidget(LABEL, 100, 45, this);
	curr_character->SetText("RAMONA", { 255,255,255,255 }, App->font->large_size);

	//Character lives left
	life = (DynamicLabel*)App->gui->CreateWidget(DYNAMIC_LABEL, 60, 45, this);
	life->SetText("x3", { 255,255,255,255 }, App->font->medium_size);
}

void j1InGameScene::HandleInput()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->entities->SetToStart();

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		App->entities->SetToStart();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
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
}

void j1InGameScene::UpdateUIpos()
{
	iPoint temp;

	//Life Icon
	temp = { 15, 40 };
	temp = App->render->ScreenToWorld(temp.x, temp.y);
	App->render->Blit(hud_tex, temp.x, temp.y, &char1_life_icon);

	//Character's name label
	temp = { 100, 45 };
	curr_character->position = App->render->ScreenToWorld(temp.x, temp.y);

	if (App->entities->player1)
	//life->ChangeContent

	//Time Icon
	temp = { 450, 40 };
	temp = App->render->ScreenToWorld(temp.x, temp.y);
	App->render->Blit(hud_tex, temp.x, temp.y, &time_icon);
}
