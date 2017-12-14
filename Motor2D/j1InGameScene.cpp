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
	
	UpdateUI();
	App->gui->Draw();

	App->entities->Draw();
	
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(),
		map_coordinates.x, map_coordinates.y);

	UpdateTimer();
	return true;
}

bool j1InGameScene::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool j1InGameScene::CleanUp()
{
	App->tex->UnLoad(hud_tex);
	
	p2List_item<Widget*>* ui_iterator = App->gui->ui_elems.end;
	while (ui_iterator)
	{
		App->gui->DestroyWidget(ui_iterator->data);
		ui_iterator = ui_iterator->prev;
	}
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

	App->entities->Start();
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

	//Score Label
	score = (DynamicLabel*)App->gui->CreateWidget(DYNAMIC_LABEL, 15, 115, this);
	score->SetText("Score:", { 255,255,255,255 }, App->font->medium_size);

	//Coins label
	coins = (DynamicLabel*)App->gui->CreateWidget(DYNAMIC_LABEL, 800, 45, this);
	coins->SetText("Coins:", { 255, 255, 255, 255 }, App->font->medium_size);

	//Mini Tutorial Window
	tuto_window = (UIWindow*)App->gui->CreateWidget(WINDOW, 250, 1200, this);
	tuto_window->SetWindowType(HORIZONTAL_WINDOW);

	tuto_window_content1 = (Label*)App->gui->CreateWidget(LABEL, 255, 1250, this);
	tuto_window_content1->SetText("PRESS ~SPACE~ TO PERFORM A REGULAR JUMP", { 255,255,255,255 }, App->font->small_size);
	tuto_window->Attach(tuto_window_content1, { 15, 60 });

	tuto_window_content2 = (Label*)App->gui->CreateWidget(LABEL, 255, 1250, this);
	tuto_window_content2->SetText("PRESS ~SPACE~ AGAIN WHILE IN AIR TO", { 255,255,255,255 }, App->font->small_size);
	tuto_window->Attach(tuto_window_content2, { 15, 100 });

	tuto_window_content3 = (Label*)App->gui->CreateWidget(LABEL, 255, 1250, this);
	tuto_window_content3->SetText("PERFORM A DOUBLE JUMP!", { 255,255,255,255 }, App->font->small_size);
	tuto_window->Attach(tuto_window_content3, { 15, 130 });

	tuto_window_content4 = (Label*)App->gui->CreateWidget(LABEL, 255, 1250, this);
	tuto_window_content4->SetText("*STARTER'S HINT*", { 255,255,255,255 }, App->font->small_size);
	tuto_window->Attach(tuto_window_content4, { 130, 20 });
	
	//Character lives left
	life = (DynamicLabel*)App->gui->CreateWidget(DYNAMIC_LABEL, 70, 75, this);
	life->SetText("x3", { 255,255,255,255 }, App->font->medium_size);

	//Scene timer
	p2SString _timer("%i", current_time);
	time = (DynamicLabel*)App->gui->CreateWidget(DYNAMIC_LABEL, 490, 40, this);
	time->SetText(_timer.GetString(), { 255,255,255,255 }, App->font->small_size);
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
		App->sceneswitch->SwitchMap("Level2inprogress.tmx");
	
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

void j1InGameScene::UpdateUI()
{
	iPoint temp;
	p2SString temp_string;

	//Life Icon
	temp = { 15, 40 };
	temp = App->render->ScreenToWorld(temp.x, temp.y);
	App->render->Blit(hud_tex, temp.x, temp.y, &char1_life_icon);

	//Character's name label
	temp = { 100, 45 };
	curr_character->position = App->render->ScreenToWorld(temp.x, temp.y);

	//Score num label
	temp = { 15, 115 };
	score->position = App->render->ScreenToWorld(temp.x, temp.y);
	temp_string.create("Score:%i", App->entities->player1->score);
	score->ChangeContent(temp_string.GetString());

	//Life num Label
	temp = { 70, 75 };
	life->position = App->render->ScreenToWorld(temp.x, temp.y);
	temp_string.create("x%i", App->entities->player1->lives_left);
	life->ChangeContent(temp_string.GetString());

	//Time Icon
	temp = { 450, 40 };
	temp = App->render->ScreenToWorld(temp.x, temp.y);
	App->render->Blit(hud_tex, temp.x, temp.y, &time_icon);

	//Time Label
	temp = { 510, 50 };
	time->position = App->render->ScreenToWorld(temp.x, temp.y);
	temp_string.create("%i", current_time);
	time->ChangeContent(temp_string.GetString());

	//Coins num Label
	temp = { 15, 150 };
	coins->position = App->render->ScreenToWorld(temp.x, temp.y);
	temp_string.create("Coins:%i", App->entities->player1->coins);
	coins->ChangeContent(temp_string.GetString());
}

void j1InGameScene::UpdateTimer()
{
	//Updating Timer

	timer_count = scene_timer.ReadSec();
	current_time = max_time - timer_count;
	if (current_time == 0)
	{
		LOG("Now you'd go to Main Scene!");
		ResetTimer();
	}
}

void j1InGameScene::ResetTimer()
{
	current_time = max_time;
}

void j1InGameScene::UpdateScore()
{
	App->entities->player1 += current_time * 100 + App->entities->player1->lives_left * 100;
}
