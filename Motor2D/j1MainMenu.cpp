#include "j1App.h"
#include "j1MainMenu.h"
#include "j1GuiController.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1SceneSwitch.h"
#include "j1InGameScene.h"

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
		break;
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

}

void j1MainMenu::CreateCreditsWindow()
{

}
