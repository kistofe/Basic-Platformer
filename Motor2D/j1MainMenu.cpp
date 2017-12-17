#include "j1App.h"
#include "j1MainMenu.h"
#include "j1GuiController.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1InGameScene.h"
#include "j1CharacterSel.h"
#include "j1SceneSwitch.h"
#include "j1EntityManager.h"
#include "j1Audio.h"

j1MainMenu::j1MainMenu()
{
	name.create("main_menu");

	config		= App->LoadUiConfig(ui_elements);
	data		= config.child("main_menu");
	textures	= data.child("textures");
	labels		= data.child("labels");
	buttons		= data.child("buttons");
	windows		= data.child("windows");
}

j1MainMenu::~j1MainMenu()
{}

bool j1MainMenu::Start()
{
	AddUiElems();
	App->audio->PlayMusic("audio/music/MainMenu.ogg");
	background = App->tex->Load(textures.child("background").attribute("source").as_string());
	title = App->tex->Load(textures.child("title").attribute("source").as_string());
	github_logo = App->tex->Load("gui/GithubLogo.png");
	example_sfx = App->audio->LoadFx("audio/sfx/Jumping.wav");
	return true;
}

bool j1MainMenu::Update(float d_time)
{
	
	App->render->Blit(background, textures.child("backgroundpos_x").attribute("value").as_int(), textures.child("backgroundpos_y").attribute("value").as_int());
	App->render->Blit(title, textures.child("titlepos_x").attribute("value").as_int(), textures.child("titlepos_y").attribute("value").as_int());
	App->render->Blit(github_logo, 10, 650);
	
	App->gui->Draw();
	

	return true;
}

bool j1MainMenu::CleanUp()
{
	App->tex->UnLoad(background);
	App->tex->UnLoad(title);

	return true;
}

bool j1MainMenu::OnEvent(Button* button)
{
	bool ret = true;
	p2SString temp;

	switch (button->button_type)
	{
	case NEW_GAME: 
		App->sceneswitch->SwitchScene(App->charactersel, this);
		break;
	case LOAD_GAME: 
		App->sceneswitch->SwitchScene(App->ingamescene, this);
		App->loading_game = true;
		break;
	case SETTINGS: 
		OpenWindow(1);
		break;
	case M_VOLUME_UP:
		if (m_volume_value < 9)
		{
			m_volume_value++;
			temp.create("%i", m_volume_value);
			m_volume->ChangeContent(temp.GetString());
			App->audio->music_vol += App->audio->volume_modifier;
			Mix_VolumeMusic(App->audio->music_vol);
		}
		break;
	case M_VOLUME_DOWN:
		if (m_volume_value > 0)
		{
			m_volume_value--;
			temp.create("%i", m_volume_value);
			m_volume->ChangeContent(temp.GetString());
			App->audio->music_vol -= App->audio->volume_modifier;
			Mix_VolumeMusic(App->audio->music_vol);
		}
		break;
	case S_VOLUME_UP:
		if (s_volume_value < 9)
		{
			s_volume_value++;
			temp.create("%i", s_volume_value);
			s_volume->ChangeContent(temp.GetString());
			App->audio->sfx_vol += App->audio->volume_modifier;
			App->audio->PlayFx(example_sfx, 0, App->audio->sfx_vol);
		}
		break;
	case S_VOLUME_DOWN:
		if (s_volume_value > 0)
		{
			s_volume_value--;
			temp.create("%i", s_volume_value);
			s_volume->ChangeContent(temp.GetString());
			App->audio->sfx_vol -= App->audio->volume_modifier;
			App->audio->PlayFx(example_sfx, 0, App->audio->sfx_vol);
		}
		break;
	case TOGGLE_FULLSCREEN:
		ToggleFullscreen();
		break;
	case CREDITS: 
		OpenWindow(2);
		break;
	case WEBPAGE:
		ShellExecute(0, 0, "https://kistofe.github.io/Ramona-Flowers-vs-The-Code/", 0, 0, SW_SHOW);
		App->audio->PlayFx(example_sfx, 0, App->audio->sfx_vol);
		break;
	case EXIT:
		ret = false;
		break;
	case CLOSE_WINDOW:
		CloseWindow();		
		break;
	}

	return ret;
}

void j1MainMenu::AddUiElems()
{
	//READING UI XML FILE TO CREATE THE UI

	//Start New Game Button
	pugi::xml_node temp = buttons.child("new_game");
	new_game = (Button*)App->gui->CreateWidget(BUTTON, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	new_game->SetButtonType(NEW_GAME);
	new_game->SetSection({ temp.child("idle").attribute("x").as_int() , temp.child("idle").attribute("y").as_int(), temp.child("idle").attribute("w").as_int(), temp.child("idle").attribute("h").as_int()},
	{ temp.child("hovering").attribute("x").as_int(), temp.child("hovering").attribute("y").as_int(), temp.child("hovering").attribute("w").as_int(), temp.child("hovering").attribute("h").as_int() },
	{ temp.child("clicked").attribute("x").as_int(), temp.child("clicked").attribute("y").as_int(), temp.child("clicked").attribute("w").as_int(), temp.child("clicked").attribute("h").as_int()});

	//Load Game Button
	temp = buttons.child("load_game");
	load_game = (Button*)App->gui->CreateWidget(BUTTON, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	load_game->SetButtonType(LOAD_GAME);
	pugi::xml_document doc;
	pugi::xml_parse_result save_exists = doc.load_file("save.xml");
	if (save_exists)
		load_game->SetSection({ temp.child("idle").attribute("x").as_int() , temp.child("idle").attribute("y").as_int(), temp.child("idle").attribute("w").as_int(), temp.child("idle").attribute("h").as_int()},
		{ temp.child("hovering").attribute("x").as_int(), temp.child("hovering").attribute("y").as_int(), temp.child("hovering").attribute("w").as_int(), temp.child("hovering").attribute("h").as_int() },
		{ temp.child("clicked").attribute("x").as_int(), temp.child("clicked").attribute("y").as_int(), temp.child("clicked").attribute("w").as_int(), temp.child("clicked").attribute("h").as_int() });
	if (!save_exists)
	{
		load_game->SetSection({ temp.child("idle").attribute("x").as_int(), temp.child("idle").attribute("y").as_int(), temp.child("idle").attribute("w").as_int(), temp.child("idle").attribute("h").as_int()},
		{ temp.child("hovering").attribute("x").as_int(), temp.child("hovering").attribute("y").as_int(), temp.child("hovering").attribute("w").as_int(), temp.child("hovering").attribute("h").as_int() },
		{ temp.child("clicked").attribute("x").as_int(), temp.child("clicked").attribute("y").as_int(), temp.child("clicked").attribute("w").as_int(), temp.child("clicked").attribute("h").as_int() }, 
		{ temp.child("disabled").attribute("x").as_int(), temp.child("disabled").attribute("y").as_int(), temp.child("disabled").attribute("w").as_int(), temp.child("disabled").attribute("h").as_int() });
	}
		
	//Settings Button
	temp = buttons.child("settings");
	settings = (Button*)App->gui->CreateWidget(BUTTON, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	settings->SetButtonType(SETTINGS);
	settings->SetSection({ temp.child("idle").attribute("x").as_int() , temp.child("idle").attribute("y").as_int(), temp.child("idle").attribute("w").as_int(), temp.child("idle").attribute("h").as_int() },
	{ temp.child("hovering").attribute("x").as_int(), temp.child("hovering").attribute("y").as_int(), temp.child("hovering").attribute("w").as_int(), temp.child("hovering").attribute("h").as_int() },
	{ temp.child("clicked").attribute("x").as_int(), temp.child("clicked").attribute("y").as_int(), temp.child("clicked").attribute("w").as_int(), temp.child("clicked").attribute("h").as_int() });
	
	//Credits Button
	temp = buttons.child("credits");
	credits = (Button*)App->gui->CreateWidget(BUTTON, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	credits->SetButtonType(CREDITS);
	credits->SetSection({ temp.child("idle").attribute("x").as_int() , temp.child("idle").attribute("y").as_int(), temp.child("idle").attribute("w").as_int(), temp.child("idle").attribute("h").as_int() },
	{ temp.child("hovering").attribute("x").as_int(), temp.child("hovering").attribute("y").as_int(), temp.child("hovering").attribute("w").as_int(), temp.child("hovering").attribute("h").as_int() },
	{ temp.child("clicked").attribute("x").as_int(), temp.child("clicked").attribute("y").as_int(), temp.child("clicked").attribute("w").as_int(), temp.child("clicked").attribute("h").as_int() });

	//Exit Button
	temp = buttons.child("exit");
	exit = (Button*)App->gui->CreateWidget(BUTTON, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	exit->SetButtonType(EXIT);
	exit->SetSection({ temp.child("idle").attribute("x").as_int() , temp.child("idle").attribute("y").as_int(), temp.child("idle").attribute("w").as_int(), temp.child("idle").attribute("h").as_int() },
	{ temp.child("hovering").attribute("x").as_int(), temp.child("hovering").attribute("y").as_int(), temp.child("hovering").attribute("w").as_int(), temp.child("hovering").attribute("h").as_int() },
	{ temp.child("clicked").attribute("x").as_int(), temp.child("clicked").attribute("y").as_int(), temp.child("clicked").attribute("w").as_int(), temp.child("clicked").attribute("h").as_int() });

	//New Game label
	temp = labels.child("new_game");
	new_game_lab = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	new_game_lab->SetText(temp.child("content").attribute("value").as_string(), { 255, 255,255,255 }, App->font->medium_size);

	//Load Game Label
	temp = labels.child("load_game");
	load_game_lab = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	load_game_lab->SetText(temp.child("content").attribute("value").as_string(), { 255,255,255,255 }, App->font->medium_size);

	//Settings Label
	temp = labels.child("settings");
	settings_lab = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	settings_lab->SetText(temp.child("content").attribute("value").as_string(), { 255,255,255,255 }, App->font->medium_size);

	//Credits Label
	temp = labels.child("credits");
	credits_lab = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	credits_lab->SetText(temp.child("content").attribute("value").as_string(), { 255, 255, 255, 255 }, App->font->medium_size);

	//Exit Label
	temp = labels.child("exit");
	exit_lab = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	exit_lab->SetText(temp.child("content").attribute("value").as_string(), { 255,255,255,255 }, App->font->medium_size);

	//Copyright label
	temp = labels.child("copyright");
	copyright = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	copyright->SetText(temp.child("content").attribute("value").as_string(), { 255,255,255,255 }, App->font->small_size);

	//Webpage button
	webpage = (Button*)App->gui->CreateWidget(BUTTON, 10, 650, this);
	webpage->SetButtonType(WEBPAGE);
	webpage->SetSection({ 955, 859, 102, 102 }, { 955, 859, 102, 102 }, { 955, 859, 102, 102 });
	webpage->SetArea(102, 102);
}

void j1MainMenu::OpenWindow(uint type)
{
	pugi::xml_node temp;

	switch (type)
	{
		case 1: //Creating Settings Window
		if (opened_win == nullptr)
		{
			//Main Window
			temp = windows.child("settings");
			settings_win = (UIWindow*)App->gui->CreateWidget(WINDOW, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
			settings_win->SetWindowType(VERTICAL_WINDOW);
			//settings_win->draggable = temp.child("draggable").attribute("value").as_bool();
			settings_win->draggable = false;

			//Title Small window
			temp = windows.child("title");
			title_win = (UIWindow*)App->gui->CreateWidget(WINDOW, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
			title_win->SetWindowType(TITLE_WINDOW);
			settings_win->Attach(title_win, { temp.child("relative_pos_x").attribute("value").as_int(), temp.child("relative_pos_y").attribute("value").as_int() });

			//Settings Title Label
			temp = labels.child("settings_title");
			settings_title_lab = (Label*)App->gui->CreateWidget(LABEL, 318, 405, this);
			settings_title_lab->SetText(temp.child("content").attribute("value").as_string(), { 255,255,255,255 }, App->font->medium_size);
			title_win->Attach(settings_title_lab, { temp.child("relative_pos_x").attribute("value").as_int(), temp.child("relative_pos_y").attribute("value").as_int() });

			//Close Window Button
			temp = buttons.child("close_window");
			close_window = (Button*)App->gui->CreateWidget(BUTTON, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
			close_window->SetButtonType(CLOSE_WINDOW);
			close_window->SetSection({ temp.child("idle").attribute("x").as_int() , temp.child("idle").attribute("y").as_int(), temp.child("idle").attribute("w").as_int(), temp.child("idle").attribute("h").as_int() },
			{ temp.child("hovering").attribute("x").as_int(), temp.child("hovering").attribute("y").as_int(), temp.child("hovering").attribute("w").as_int(), temp.child("hovering").attribute("h").as_int() },
			{ temp.child("clicked").attribute("x").as_int(), temp.child("clicked").attribute("y").as_int(), temp.child("clicked").attribute("w").as_int(), temp.child("clicked").attribute("h").as_int()});
			settings_win->Attach(close_window, { temp.child("relative_pos_x").attribute("value").as_int(), temp.child("relative_pos_y").attribute("value").as_int() });
			
			//Music volume down button
			m_volume_down = (Button*)App->gui->CreateWidget(BUTTON, 0, 0, this);
			m_volume_down->SetButtonType(M_VOLUME_DOWN);
			m_volume_down->SetSection({ 532, 717, 64, 32 }, { 532, 755, 64, 32 }, { 532, 793, 64, 32 });
			settings_win->Attach(m_volume_down, { 27, 95 });

			//Music volume down label
			m_volume_minus = (Label*)App->gui->CreateWidget(LABEL, 0, 0, this);
			m_volume_minus->SetText("-", { 255,255,255,255 }, App->font->medium_size);
			m_volume_down->Attach(m_volume_minus, { 22, 2 });

			//Music volume up button
			m_volume_up = (Button*)App->gui->CreateWidget(BUTTON, 0, 0, this);
			m_volume_up->SetButtonType(M_VOLUME_UP);
			m_volume_up->SetSection({ 532, 717, 64, 32 }, { 532, 755, 64, 32 }, { 532, 793, 64, 32 });
			settings_win->Attach(m_volume_up, { 177, 95 });

			//Music volume up label
			m_volume_plus = (Label*)App->gui->CreateWidget(LABEL, 0, 0, this);
			m_volume_plus->SetText("+", { 255,255,255,255 }, App->font->medium_size);
			m_volume_up->Attach(m_volume_plus, { 22, 2 });

			//Music volume value label
			m_volume = (DynamicLabel*)App->gui->CreateWidget(DYNAMIC_LABEL, 0, 0, this);
			m_volume->SetText("9", { 255,255,255,255 }, App->font->medium_size);
			settings_win->Attach(m_volume, { 125, 95 });

			//Music volume label
			music_volume = (Label*)App->gui->CreateWidget(LABEL, 0, 0, this);
			music_volume->SetText("Music volume:", { 255,255,255,255 }, App->font->small_size);
			settings_win->Attach(music_volume, { 27, 72 });

			//SFX volume down button
			s_volume_down = (Button*)App->gui->CreateWidget(BUTTON, 0, 0, this);
			s_volume_down->SetButtonType(S_VOLUME_DOWN);
			s_volume_down->SetSection({ 532, 717, 64, 32 }, { 532, 755, 64, 32 }, { 532, 793, 64, 32 });
			settings_win->Attach(s_volume_down, { 27, 165 });

			//SFX volume down label
			s_volume_minus = (Label*)App->gui->CreateWidget(LABEL, 0, 0, this);
			s_volume_minus->SetText("-", { 255,255,255,255 }, App->font->medium_size);
			s_volume_down->Attach(s_volume_minus, { 22, 2 });

			//SFX volume up button
			s_volume_up = (Button*)App->gui->CreateWidget(BUTTON, 0, 0, this);
			s_volume_up->SetButtonType(S_VOLUME_UP);
			s_volume_up->SetSection({ 532, 717, 64, 32 }, { 532, 755, 64, 32 }, { 532, 793, 64, 32 });
			settings_win->Attach(s_volume_up, { 177, 165 });

			//SFX volume up label
			s_volume_plus = (Label*)App->gui->CreateWidget(LABEL, 0, 0, this);
			s_volume_plus->SetText("+", { 255,255,255,255 }, App->font->medium_size);
			s_volume_up->Attach(s_volume_plus, { 22, 2 });

			//SFX volume value label
			s_volume = (DynamicLabel*)App->gui->CreateWidget(DYNAMIC_LABEL, 0, 0, this);
			s_volume->SetText("9", { 255,255,255,255 }, App->font->medium_size);
			settings_win->Attach(s_volume, { 125, 165 });

			//SFX volume label
			sfx_volume = (Label*)App->gui->CreateWidget(LABEL, 0, 0, this);
			sfx_volume->SetText("SFX volume:", { 255,255,255,255 }, App->font->small_size);
			settings_win->Attach(sfx_volume, { 27, 142 });

			//Toggle fullscreen button
			toggle_fullscreen = (Button*)App->gui->CreateWidget(BUTTON, 0, 0, this);
			toggle_fullscreen->SetButtonType(TOGGLE_FULLSCREEN);
			toggle_fullscreen->SetSection({ 532, 717, 64, 32 }, { 532, 755, 64, 32 }, { 532, 793, 64, 32 });
			settings_win->Attach(toggle_fullscreen, { 100, 240 });

			//Toggle fullscreen label
			fullscreen_lab = (Label*)App->gui->CreateWidget(LABEL, 0, 0, this);
			fullscreen_lab->SetText("Fullscreen:", { 255,255,255,255 }, App->font->small_size);
			settings_win->Attach(fullscreen_lab, { 27, 215 });

			opened_win = settings_win;
		}
		break;
		case 2: //Creating Credits Window
		if (opened_win == nullptr)
		{
			//Main Window
			temp = windows.child("credits");
			credits_win = (UIWindow*)App->gui->CreateWidget(WINDOW, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
			credits_win->SetWindowType(VERTICAL_WINDOW);
			credits_win->draggable = true;

			//Title small window
			temp = windows.child("title");
			title_win = (UIWindow*)App->gui->CreateWidget(WINDOW, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
			title_win->SetWindowType(TITLE_WINDOW);
			credits_win->Attach(title_win, { temp.child("relative_pos_x").attribute("value").as_int(), temp.child("relative_pos_y").attribute("value").as_int() });

			//Credits Title Label
			temp = labels.child("credits_title");
			credits_title_lab = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
			credits_title_lab->SetText(temp.child("content").attribute("value").as_string(), { 255,255,255,255 }, App->font->medium_size);
			title_win->Attach(credits_title_lab, { temp.child("relative_pos_x").attribute("value").as_int(), temp.child("relative_pos_y").attribute("value").as_int() });

			//Close Window Button
			temp = buttons.child("close_window");
			close_window = (Button*)App->gui->CreateWidget(BUTTON, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
			close_window->SetButtonType(CLOSE_WINDOW);
			close_window->SetSection({ temp.child("idle").attribute("x").as_int() , temp.child("idle").attribute("y").as_int(), temp.child("idle").attribute("w").as_int(), temp.child("idle").attribute("h").as_int() },
			{ temp.child("hovering").attribute("x").as_int(), temp.child("hovering").attribute("y").as_int(), temp.child("hovering").attribute("w").as_int(), temp.child("hovering").attribute("h").as_int() },
			{ temp.child("clicked").attribute("x").as_int(), temp.child("clicked").attribute("y").as_int(), temp.child("clicked").attribute("w").as_int(), temp.child("clicked").attribute("h").as_int() });
			credits_win->Attach(close_window, { temp.child("relative_pos_x").attribute("value").as_int(), temp.child("relative_pos_y").attribute("value").as_int() });
		
			//First label
			credits_1 = (Label*)App->gui->CreateWidget(LABEL, 0, 0, this);
			credits_1->SetText("Programming: Cristian Palos", { 255,255,255,255 }, App->font->small_size);
			credits_win->Attach(credits_1, { 17, 85 });

			//Second label
			credits_2 = (Label*)App->gui->CreateWidget(LABEL, 0, 0, this);
			credits_2->SetText("Joel Cabaco", { 255,255,255,255 }, App->font->small_size);
			credits_win->Attach(credits_2, { 135, 105 });

			//Third label
			credits_3 = (Label*)App->gui->CreateWidget(LABEL, 0, 0, this);
			credits_3->SetText("Sprites from:", { 255,255,255,255 }, App->font->small_size);
			credits_win->Attach(credits_3, { 17, 145 });

			opened_win = credits_win;
		}
		break;
	}
}


void j1MainMenu::CloseWindow()
{
	p2List_item<Widget*>* ui_iterator = App->gui->ui_elems.start;
	while (ui_iterator)
	{
		if (ui_iterator->data->type == WINDOW && ui_iterator->data)
		{
			App->gui->DestroyWidget(ui_iterator->data);
			break;
		}
		ui_iterator = ui_iterator->next;
	}

	opened_win = nullptr;
}

void j1MainMenu::ToggleFullscreen()
{
	if (!fullscreen_on)
	{
		fullscreen_on = true;
		SDL_SetWindowFullscreen(App->win->window, SDL_WINDOW_FULLSCREEN);
	}

	else if (fullscreen_on)
	{
		fullscreen_on = false;
		SDL_SetWindowFullscreen(App->win->window, 0);
	}

	return;
}

