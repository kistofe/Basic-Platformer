#include "j1App.h"
#include "j1MainMenu.h"
#include "j1GuiController.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1SceneSwitch.h"
#include "j1InGameScene.h"
#include "Widget.h"

j1MainMenu::j1MainMenu()
{
	name.create("main_menu");
}

j1MainMenu::~j1MainMenu()
{}

bool j1MainMenu::Start()
{
	if (active)
		AddUiElems();

	background = App->tex->Load("gui/Background.png");
	title = App->tex->Load("gui/title.png");
	
	return true;
}

bool j1MainMenu::Update(float d_time)
{
	App->render->Blit(background, -200, 1035);
	App->render->Blit(title, 0, 1050);
	
	App->gui->Draw();
	

	return true;
}

bool j1MainMenu::CleanUp()
{
	App->tex->UnLoad(background);
	App->tex->UnLoad(title);

	return true;
}

bool j1MainMenu::OnEvent(Button * button)
{
	bool ret = true;

	switch (button->button_type)
	{
	case NEW_GAME: 
		App->sceneswitch->SwitchScene(App->ingamescene, this);
		break;
	case LOAD_GAME: //load if save exists
		break;
	case SETTINGS: 
		CreateSettingsWindow();
		break;
	case CREDITS: 
		CreateCreditsWindow();
		break;
	case EXIT:
		ret = false;
		break;
	case CLOSE_WINDOW:
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
		break;
	}
		
	}

	return ret;
}

void j1MainMenu::AddUiElems()
{
	//Start New Game Button
	new_game = (Button*)App->gui->CreateWidget(BUTTON, 45, 1450, this);
	new_game->SetButtonType(NEW_GAME);
	new_game->SetSection({ 3, 7, 289, 96 }, { 3, 103, 289, 96 }, { 3, 199, 289, 96 });

	//Load Game Button
	load_game = (Button*)App->gui->CreateWidget(BUTTON, 360, 1450, this);
	load_game->SetButtonType(LOAD_GAME);
	load_game->SetSection({ 318, 7, 289, 96 }, { 318, 103, 289, 96 }, { 318, 199, 289, 96 });

	//Settings Button
	settings = (Button*)App->gui->CreateWidget(BUTTON, 45, 1570, this);
	settings->SetButtonType(SETTINGS);
	settings->SetSection({ 3, 319, 289, 96 }, { 3, 415, 289, 96 }, { 3, 511, 289, 96 });

	//Credits Button
	credits = (Button*)App->gui->CreateWidget(BUTTON, 360, 1570, this);
	credits->SetButtonType(CREDITS);
	credits->SetSection({ 647, 7, 289, 96 }, { 647, 103, 289, 96 }, { 647, 199, 289, 96 });

	//Exit Button
	exit = (Button*)App->gui->CreateWidget(BUTTON, 250, 1670, this);
	exit->SetButtonType(EXIT);
	exit->SetSection({ 318, 319, 289, 96 }, { 318, 415, 289, 96 }, { 318, 511, 289, 96 });

	//New Game label
	new_game_lab = (Label*)App->gui->CreateWidget(LABEL, 140, 1483, this);
	new_game_lab->SetText("NEW GAME", { 255,255,255,255 }, App->font->medium_size);

	//Load Game Label
	load_game_lab = (Label*)App->gui->CreateWidget(LABEL, 455, 1483, this);
	load_game_lab->SetText("LOAD GAME", { 255,255,255,255 }, App->font->medium_size);

	//Settings Label
	settings_lab = (Label*)App->gui->CreateWidget(LABEL, 140, 1603, this);
	settings_lab->SetText("SETTINGS", { 255,255,255,255 }, App->font->medium_size);

	//Credits Label
	credits_lab = (Label*)App->gui->CreateWidget(LABEL, 455, 1603, this);
	credits_lab->SetText("CREDITS", { 255, 255, 255, 255 }, App->font->medium_size);

	//Exit Label
	exit_lab = (Label*)App->gui->CreateWidget(LABEL, 354, 1703, this);
	exit_lab->SetText("EXIT", { 255,255,255,255 }, App->font->medium_size);

	//Copyright label
	copyright = (Label*)App->gui->CreateWidget(LABEL, 550, 1770, this);
	copyright->SetText("All rights reserved - Edgypoint Castellbisbal -", { 255,255,255,255 }, App->font->small_size);

}

void j1MainMenu::CreateSettingsWindow()
{
	//Main Window
	settings_win = (UIWindow*)App->gui->CreateWidget(WINDOW, 250, 1400, this);
	settings_win->SetWindowType(VERTICAL_WINDOW);
	settings_win->SetArea();
	settings_win->draggable = true;

	//Title Small window
	title_win = (UIWindow*)App->gui->CreateWidget(WINDOW, 285, 1390, this);
	title_win->SetWindowType(TITLE_WINDOW);
	title_win->SetArea();
	settings_win->Attach(title_win, { 35,-10 });

	//Settings Label
	settings_lab = (Label*)App->gui->CreateWidget(LABEL, 318, 1405, this);
	settings_lab->SetText("SETTINGS", { 255,255,255,255 }, App->font->medium_size);
	title_win->Attach(settings_lab, { 33, 15 });

	//Close Window Button
	close_window = (Button*)App->gui->CreateWidget(BUTTON, 520, 1400, this);
	close_window->SetButtonType(CLOSE_WINDOW);
	close_window->SetSection({ 436, 645, 52, 64 }, { 499, 645, 52, 64 }, { 557, 645, 52, 64 });
	settings_win->Attach(close_window, { 270, 0 });
}

void j1MainMenu::CreateCreditsWindow()
{
	//Main Window
	credits_win = (UIWindow*)App->gui->CreateWidget(WINDOW, 250, 1400, this);
	credits_win->SetWindowType(VERTICAL_WINDOW);
	credits_win->SetArea();
	credits_win->draggable = true;
	
	//Title small window
	title_win = (UIWindow*)App->gui->CreateWidget(WINDOW, 285, 1390, this);
	title_win->SetWindowType(TITLE_WINDOW);
	title_win->SetArea();
	credits_win->Attach(title_win, { 35, -10 });

	//Credits Label
	credits_lab = (Label*)App->gui->CreateWidget(LABEL, 318, 1405, this);
	credits_lab->SetText("CREDITS", { 255,255,255,255 }, App->font->medium_size);
	title_win->Attach(credits_lab, { 33, 15 });

	//Close window Button
	close_window = (Button*)App->gui->CreateWidget(BUTTON, 520, 1400, this);
	close_window->SetButtonType(CLOSE_WINDOW);
	close_window->SetSection({ 436, 645, 52, 64 }, { 499, 645, 52, 64 }, { 557, 645, 52, 64 });
	credits_win->Attach(close_window, { 270, 0 });
}

