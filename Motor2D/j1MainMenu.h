#ifndef _MAINMENU_
#define _MAINMENU_

#include "j1Module.h"
#include "j1GuiController.h"

#include "SDL\include\SDL.h"

class j1MainMenu : public j1Module
{

public:
	j1MainMenu();
	~j1MainMenu();

	bool Start();
	bool Update(float d_time);
	bool CleanUp();

	bool OnEvent(Button* button);

private:

	void AddUiElems();
	void OpenWindow(uint type);
	void CloseWindow();
	void ToggleFullscreen();

public:

	//Main Buttons
	Button* new_game = nullptr;
	Button* load_game = nullptr;
	Button* settings = nullptr;
	Button* credits = nullptr;
	Button* exit = nullptr;
	Button* close_window = nullptr;
	Button* m_volume_up = nullptr;
	Button* m_volume_down = nullptr;
	Button* s_volume_up = nullptr;
	Button* s_volume_down = nullptr;
	Button* toggle_fullscreen = nullptr;

	//Dynamic labels
	DynamicLabel* m_volume = nullptr;
	DynamicLabel* s_volume = nullptr;

	//Labels
	Label* new_game_lab = nullptr;
	Label* load_game_lab = nullptr;
	Label* settings_lab = nullptr;
	Label* credits_lab = nullptr;
	Label* exit_lab = nullptr;
	Label* settings_title_lab = nullptr;
	Label* credits_title_lab = nullptr;
	Label* credits_1 = nullptr;
	Label* credits_2 = nullptr;
	Label* credits_3 = nullptr;
	Label* copyright = nullptr;
	Label* m_volume_plus = nullptr;
	Label* m_volume_minus = nullptr;
	Label* s_volume_plus = nullptr;
	Label* s_volume_minus = nullptr;
	Label* music_volume = nullptr;
	Label* sfx_volume = nullptr;
	Label* fullscreen_lab = nullptr;

	//Windows
	UIWindow* settings_win = nullptr;
	UIWindow* credits_win = nullptr;
	UIWindow* title_win = nullptr;

	//Images
	SDL_Texture* background = nullptr;
	SDL_Texture* title = nullptr;

private:

	pugi::xml_document ui_elements;
	pugi::xml_node config;
	pugi::xml_node data;
	pugi::xml_node textures;
	pugi::xml_node labels;
	pugi::xml_node buttons;
	pugi::xml_node windows;

	bool fullscreen_on = false;
	int m_volume_value = 9;
	int s_volume_value = 9;
	UIWindow* opened_win = nullptr;
		
};
#endif
