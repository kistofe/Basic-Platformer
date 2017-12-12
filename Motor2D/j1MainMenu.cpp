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
	new_game->SetSection({ 10, 8, 192, 64 }, { 10, 72, 192, 64 }, { 10, 136, 192, 64 });

	//Load Game Button
	load_game = (Button*)App->gui->CreateWidget(BUTTON, 50, 1650, this);
	load_game->SetButtonType(LOAD_GAME);

	//Settings Button
	settings = (Button*)App->gui->CreateWidget(BUTTON, 200, 1750, this);
	settings->SetButtonType(SETTINGS);

	//Credits Button
	credits = (Button*)App->gui->CreateWidget(BUTTON, 200, 1850, this);
	credits->SetButtonType(CREDITS);

	//Exit Button
	exit = (Button*)App->gui->CreateWidget(BUTTON, 200, 1950, this);
	exit->SetButtonType(EXIT);

	//New Game label
	new_game_lab = (Label*)App->gui->CreateWidget(LABEL, 50, 1450, this);
	new_game_lab->SetText("NEW GAME", { 255,255,255,255 }, App->font->medium_size);
}

void j1MainMenu::CreateSettingsWindow()
{

}

void j1MainMenu::CreateCreditsWindow()
{

}
