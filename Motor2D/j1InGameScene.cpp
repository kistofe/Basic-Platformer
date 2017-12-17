#include "j1App.h"
#include "j1InGameScene.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1SceneSwitch.h"
#include "j1Pathfinding.h"
#include "j1Textures.h"
#include "j1CharacterSel.h"
#include "j1MainMenu.h"


j1InGameScene::j1InGameScene()
{
	name.create("ingame_scene");

	config = App->LoadUiConfig(ui_elements);
	data = config.child("ingame_scene");
	textures = data.child("textures");
	labels = data.child("labels");
	dynamic_labels = data.child("dynamic_labels");
	windows = data.child("windows");
	buttons = data.child("buttons");
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

	char1_life_icon = { textures.child("char1_icon").attribute("x").as_int(), textures.child("char1_icon").attribute("y").as_int(), textures.child("char1_icon").attribute("w").as_int(), textures.child("char1_icon").attribute("h").as_int() };
	char2_life_icon = { textures.child("char2_icon").attribute("x").as_int(), textures.child("char2_icon").attribute("y").as_int(), textures.child("char2_icon").attribute("w").as_int(), textures.child("char2_icon").attribute("h").as_int() };
	time_icon = { textures.child("time_icon").attribute("x").as_int(), textures.child("time_icon").attribute("y").as_int(), textures.child("time_icon").attribute("w").as_int(), textures.child("time_icon").attribute("h").as_int() };

	return ret;
}

bool j1InGameScene::Start()
{
	InitializeMap(App->map->map_name.start->data->GetString());
	
//	App->audio->PlayMusic("audio/music/Level_1.ogg");
	hud_tex = App->tex->Load(textures.child("hud").attribute("source").as_string());

	scene_timer.Start();
	AddUiElems();
			
	return true;
}

bool j1InGameScene::Update(float d_time)
{
	if (App->loading_game)
		App->LoadGame(); 

	HandleInput();
	App->map->Draw();
	UpdateUI();
	App->gui->Draw();
	App->entities->Draw();
	UpdateTimer();

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

bool j1InGameScene::CleanUp()
{
	App->tex->UnLoad(hud_tex);
	
	return true;
}

bool j1InGameScene::Load(pugi::xml_node& data)
{
	saved_time = data.child("current_time").attribute("value").as_uint();

	current_time = saved_time;

	return true;
}

bool j1InGameScene::Save(pugi::xml_node& data) const
{
	data.append_child("current_time").append_attribute("value") = current_time;

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

bool j1InGameScene::OnEvent(Button* button)
{
	bool ret = true;

	switch (button->button_type)
	{
	case RESUME:
		ResumeGame();
		break;
	case TO_MAIN_SCENE:
		App->sceneswitch->SwitchScene(App->mainmenu, this);
		paused = false;
		break;
	
	}

	return ret;
}

void j1InGameScene::AddUiElems()
{
	//READING UI XML FILE TO CREATE THE UI


	//Current Character Label
	 pugi::xml_node temp = dynamic_labels.child("current_character");
	curr_character = (DynamicLabel*)App->gui->CreateWidget(DYNAMIC_LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	if (App->entities->player1->sel_char == 0)
		curr_character->SetText(temp.child("content1").attribute("value").as_string(), { 255,255,255,255 }, App->font->large_size);
	if (App->entities->player1->sel_char == 1)
		curr_character->SetText(temp.child("content2").attribute("value").as_string(), { 255,255,255,255 }, App->font->large_size);

	//Mini Tutorial Window
	temp = windows.child("tutorial");
	tuto_window = (UIWindow*)App->gui->CreateWidget(WINDOW, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	tuto_window->SetWindowType(HORIZONTAL_WINDOW);

	temp = labels.child("tutorial_1");
	tuto_window_content1 = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	tuto_window_content1->SetText(temp.child("content").attribute("value").as_string(), { 255,255,255,255 }, App->font->small_size);
	tuto_window->Attach(tuto_window_content1, { 15, 60 });

	temp = labels.child("tutorial_2");
	tuto_window_content2 = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	tuto_window_content2->SetText(temp.child("content").attribute("value").as_string(), { 255,255,255,255 }, App->font->small_size);
	tuto_window->Attach(tuto_window_content2, { 15, 100 });

	temp = labels.child("tutorial_3");
	tuto_window_content3 = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	tuto_window_content3->SetText(temp.child("content").attribute("value").as_string(), { 255,255,255,255 }, App->font->small_size);
	tuto_window->Attach(tuto_window_content3, { 15, 130 });

	temp = labels.child("tutorial_4");
	tuto_window_content4 = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	tuto_window_content4->SetText(temp.child("content").attribute("value").as_string(), { 255,255,255,255 }, App->font->small_size);
	tuto_window->Attach(tuto_window_content4, { 130, 20 });

	//Score Label
	temp = dynamic_labels.child("score");
	score = (DynamicLabel*)App->gui->CreateWidget(DYNAMIC_LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	score->SetText(temp.child("content").attribute("value").as_string(), { 255,255,255,255 }, App->font->medium_size);

	//Coins label
	temp = dynamic_labels.child("coins");
	coins = (DynamicLabel*)App->gui->CreateWidget(DYNAMIC_LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	coins->SetText(temp.child("content").attribute("value").as_string(), { 255, 255, 255, 255 }, App->font->medium_size);

	//Character lives left
	temp = dynamic_labels.child("life");
	life = (DynamicLabel*)App->gui->CreateWidget(DYNAMIC_LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	life->SetText(temp.child("content").attribute("value").as_string(), { 255,255,255,255 }, App->font->medium_size);

	//Scene timer
	temp = dynamic_labels.child("time");
	p2SString _timer(temp.child("content").attribute("value").as_string(), current_time);
	time = (DynamicLabel*)App->gui->CreateWidget(DYNAMIC_LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	time->SetText(_timer.GetString(), { 255,255,255,255 }, App->font->small_size);

	//Current Level
	temp = dynamic_labels.child("level");
	p2SString _level(temp.child("content").attribute("value").as_string(), current_lvl);
	level = (DynamicLabel*)App->gui->CreateWidget(DYNAMIC_LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	level->SetText(_level.GetString(), { 255,255,255,255 }, App->font->small_size);
	
}

void j1InGameScene::HandleInput()
{

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if (!paused)
			PauseGame();
		else
			ResumeGame();
	}
		
	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
		App->audio->VolumeControl();

	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
		App->audio->VolumeControl();

	if (paused)
		return;

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

}

void j1InGameScene::UpdateUI()
{
	iPoint temp;
	p2SString temp_string;

	//Life Icon
	temp = App->render->ScreenToWorld(textures.child("char_icon_pos_x").attribute("value").as_int(), textures.child("char_icon_pos_y").attribute("value").as_int());
	if (App->charactersel->selected_character == 0)
		App->render->Blit(hud_tex, temp.x, temp.y, &char1_life_icon);
	if (App->charactersel->selected_character == 1)
		App->render->Blit(hud_tex, temp.x, temp.y, &char2_life_icon);

	//Time Icon
	temp = { textures.child("time_icon_pos_x").attribute("value").as_int(), textures.child("time_icon_pos_y").attribute("value").as_int() };
	temp = App->render->ScreenToWorld(temp.x, temp.y);
	App->render->Blit(hud_tex, temp.x, temp.y, &time_icon);

	//Character's name label
	pugi::xml_node temp_node = dynamic_labels.child("current_character");
	curr_character->position = App->render->ScreenToWorld(temp_node.child("position_x").attribute("value").as_int(), temp_node.child("position_y").attribute("value").as_int());
	if (App->charactersel->selected_character == 0)
		curr_character->ChangeContent(temp_node.child("content1").attribute("value").as_string());
	if (App->charactersel->selected_character == 1)
		curr_character->ChangeContent(temp_node.child("content2").attribute("value").as_string());
			
	//Score num label
	temp_node = dynamic_labels.child("score");
	score->position = App->render->ScreenToWorld(temp_node.child("position_x").attribute("value").as_int(), temp_node.child("position_y").attribute("value").as_int());
	temp_string.create(temp_node.child("content").attribute("value").as_string(), App->entities->player1->score);
	score->ChangeContent(temp_string.GetString());

	//Life num Label
	temp_node = dynamic_labels.child("life");
	life->position = App->render->ScreenToWorld(temp_node.child("position_x").attribute("value").as_int(), temp_node.child("position_y").attribute("value").as_int());
	temp_string.create(temp_node.child("content").attribute("value").as_string(), App->entities->player1->lives_left);
	life->ChangeContent(temp_string.GetString());

	//Time Label
	temp_node = dynamic_labels.child("time");
	time->position = App->render->ScreenToWorld(temp_node.child("position_x").attribute("value").as_int(), temp_node.child("position_y").attribute("value").as_int());
	temp_string.create(temp_node.child("content").attribute("value").as_string(), current_time);
	time->ChangeContent(temp_string.GetString());

	//Coins num Label
	temp_node = dynamic_labels.child("coins");
	coins->position = App->render->ScreenToWorld(temp_node.child("position_x").attribute("value").as_int(), temp_node.child("position_y").attribute("value").as_int());
	temp_string.create(temp_node.child("content").attribute("value").as_string(), App->entities->player1->coins);
	coins->ChangeContent(temp_string.GetString());

	//Current Level 
	temp_node = dynamic_labels.child("level");
	level->position = App->render->ScreenToWorld(temp_node.child("position_x").attribute("value").as_int(), temp_node.child("position_y").attribute("value").as_int());
	temp_string.create(temp_node.child("content").attribute("value").as_string(), current_lvl);
	level->ChangeContent(temp_string.GetString());

	if (pause_window)
	{
		temp_node = windows.child("pause");
		pause_window->position = App->render->ScreenToWorld(temp_node.child("position_x").attribute("value").as_int(), temp_node.child("position_y").attribute("value").as_int());

		temp_node = windows.child("pause_title");
		pause_window_title->position = App->render->ScreenToWorld(temp_node.child("position_x").attribute("value").as_int(), temp_node.child("position_y").attribute("value").as_int());

		temp_node = labels.child("pause");
		pause_window_lab->position = App->render->ScreenToWorld(temp_node.child("position_x").attribute("value").as_int(), temp_node.child("position_y").attribute("value").as_int());

		temp_node = labels.child("resume");
		resume_lab->position = App->render->ScreenToWorld(temp_node.child("position_x").attribute("value").as_int(), temp_node.child("position_y").attribute("value").as_int());

		temp_node = labels.child("to_main_menu");
		back_lab->position = App->render->ScreenToWorld(temp_node.child("position_x").attribute("value").as_int(), temp_node.child("position_y").attribute("value").as_int());

		temp_node = buttons.child("back_to_main_menu");
		back_to_main_menu->position = App->render->ScreenToWorld(temp_node.child("position_x").attribute("value").as_int(), temp_node.child("position_y").attribute("value").as_int());

		temp_node = buttons.child("resume");
		resume->position = App->render->ScreenToWorld(temp_node.child("position_x").attribute("value").as_int(), temp_node.child("position_y").attribute("value").as_int());
	}
		
}

void j1InGameScene::UpdateTimer()
{
	if (paused)
		return;

	//Updating Timer
	timer_count = scene_timer.ReadSec();
	current_time = max_time - timer_count;
	if (current_time == 0)
	{
		ResetTimer();
		App->sceneswitch->SwitchScene(App->mainmenu, this);
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

void j1InGameScene::OpenWindow(uint type)
{
	pugi::xml_node temp;

	switch (type)
	{
		case 1: //Creating Pause Window
		{
			//Main Pause Window
			temp = windows.child("pause");
			pause_window = (UIWindow*)App->gui->CreateWidget(WINDOW, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
			pause_window->SetWindowType(HORIZONTAL_WINDOW);
			
			//Main Pause Window Title Window
			temp = windows.child("pause_title");
			pause_window_title = (UIWindow*)App->gui->CreateWidget(WINDOW, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
			pause_window_title->SetWindowType(TITLE_WINDOW);
			pause_window->Attach(pause_window_title, { temp.child("relative_pos_x").attribute("value").as_int(), temp.child("relative_pos_y").attribute("value").as_int() });

			//Label
			temp = labels.child("pause");
			pause_window_lab = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
			pause_window_lab->SetText(temp.child("content").attribute("value").as_string(), { 255, 255, 255, 255 }, App->font->large_size);
			pause_window_title->Attach(pause_window_lab, { temp.child("relative_pos_x").attribute("value").as_int(), temp.child("relative_pos_y").attribute("value").as_int() });

			//Back to main menu Button
			temp = buttons.child("back_to_main_menu");
			back_to_main_menu = (Button*)App->gui->CreateWidget(BUTTON, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
			back_to_main_menu->SetButtonType(TO_MAIN_SCENE);
			back_to_main_menu->SetSection({ temp.child("idle").attribute("x").as_int() , temp.child("idle").attribute("y").as_int(), temp.child("idle").attribute("w").as_int(), temp.child("idle").attribute("h").as_int() },
			{ temp.child("hovering").attribute("x").as_int(), temp.child("hovering").attribute("y").as_int(), temp.child("hovering").attribute("w").as_int(), temp.child("hovering").attribute("h").as_int() },
			{ temp.child("clicked").attribute("x").as_int(), temp.child("clicked").attribute("y").as_int(), temp.child("clicked").attribute("w").as_int(), temp.child("clicked").attribute("h").as_int() });
			pause_window->Attach(back_to_main_menu, { temp.child("relative_pos_x").attribute("value").as_int(), temp.child("relative_pos_y").attribute("value").as_int()});

			//Resume Label
			temp = labels.child("to_main_menu");
			back_lab = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
			back_lab->SetText(temp.child("content").attribute("value").as_string(), { 255, 255, 255, 255 }, App->font->medium_size);
			pause_window->Attach(back_lab, { temp.child("relative_pos_x").attribute("value").as_int(), temp.child("relative_pos_y").attribute("value").as_int() });

			//Resume Button
			temp = buttons.child("resume");
			resume = (Button*)App->gui->CreateWidget(BUTTON, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
			resume->SetButtonType(RESUME);
			resume->SetSection({ temp.child("idle").attribute("x").as_int() , temp.child("idle").attribute("y").as_int(), temp.child("idle").attribute("w").as_int(), temp.child("idle").attribute("h").as_int() },
			{ temp.child("hovering").attribute("x").as_int(), temp.child("hovering").attribute("y").as_int(), temp.child("hovering").attribute("w").as_int(), temp.child("hovering").attribute("h").as_int() },
			{ temp.child("clicked").attribute("x").as_int(), temp.child("clicked").attribute("y").as_int(), temp.child("clicked").attribute("w").as_int(), temp.child("clicked").attribute("h").as_int() });
			pause_window->Attach(resume, { temp.child("relative_pos_x").attribute("value").as_int(), temp.child("relative_pos_y").attribute("value").as_int() });

			//Resume Label
			temp = labels.child("resume");
			resume_lab = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
			resume_lab->SetText(temp.child("content").attribute("value").as_string(), { 255, 255, 255, 255 }, App->font->medium_size);
			pause_window->Attach(resume_lab, { temp.child("relative_pos_x").attribute("value").as_int(), temp.child("relative_pos_y").attribute("value").as_int() });
		}
		break;
	
	}
}

void j1InGameScene::PauseGame()
{
	paused = true;

	OpenWindow(1);

}

void j1InGameScene::ResumeGame()
{
	paused = false;

	App->gui->DestroyWidget(pause_window);
}


