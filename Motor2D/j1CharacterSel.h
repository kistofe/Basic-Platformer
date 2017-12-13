#ifndef _CHARACTERSELECTION_
#define _CHARACTERSELECTION_

#include "j1Module.h"
#include "j1GuiController.h"

class j1CharacterSel : public j1Module
{
public:
	j1CharacterSel();
	~j1CharacterSel();

	bool Start();
	bool Update(float d_time);
	bool CleanUp();

	bool OnEvent(Button* button);

private:

	void AddUiElems();

public:

	//Buttons
	Button* character1 = nullptr;
	Button* character2 = nullptr;

	//Labels
	Label* character1_go = nullptr;
	Label* character2_go = nullptr;
	Label* character1_name = nullptr;
	Label* character2_name = nullptr;
	Label* character1_speed_lab = nullptr;
	Label* character1_jump_lab = nullptr;
	Label* character2_speed_lab = nullptr;
	Label* character2_jump_lab = nullptr;
	
	//Windows
	UIWindow* character1_win = nullptr;
	UIWindow* character2_win = nullptr;
	UIWindow* character1_win2 = nullptr;
	UIWindow* character2_win2 = nullptr;
	
	//Images
	SDL_Texture* background = nullptr;
	SDL_Texture* title = nullptr;
	SDL_Texture* character1_portrait = nullptr;
	SDL_Texture* character2_portrait = nullptr;
};

#endif
