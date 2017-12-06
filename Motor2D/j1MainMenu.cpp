#include "j1App.h"
#include "j1MainMenu.h"
#include "j1GuiController.h"

j1MainMenu::j1MainMenu()
{
	name.create("main_menu");
}

j1MainMenu::~j1MainMenu()
{}

bool j1MainMenu::Start()
{
	return true;
}

bool j1MainMenu::CleanUp()
{
	return true;
}

bool j1MainMenu::OnEvent(Button * button)
{
	bool ret = true;

	switch (button->button_type)
	{
	case NEW_GAME: //fade to "ingame" scene
		break;
	case LOAD_GAME: //load if save exists
		break;
	case SETTINGS: //fade to "options" scene
		break;
	case CREDITS: //fade to "credits" scene
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
	new_game = (Button*)App->gui->CreateWidget(BUTTON, 200, 1550, this);
	new_game->SetButtonType(NEW_GAME);
	new_game->SetSection({ 10, 8, 192, 64 }, { 10, 72, 192, 64 }, { 10, 136, 192, 64 });

	//Load Game Button
	load_game = (Button*)App->gui->CreateWidget(BUTTON, 200, 1650, this);
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
}

void j1MainMenu::CreateSettingsWindow()
{
}

void j1MainMenu::CreateCreditsWindow()
{
}
