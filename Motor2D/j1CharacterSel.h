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
	bool CleanUp();

	bool OnEvent(Button* button);

private:

	void AddUiElems();

public:

	//Buttons
	Button* character1 = nullptr;
	Button* character2 = nullptr;
	Button* character3 = nullptr;
	Button* character4 = nullptr;

	//Labels
	Label* character1_lab = nullptr;
	Label* character2_lab = nullptr;
	Label* character3_lab = nullptr;
	Label* character4_lab = nullptr;

	//Windows
	UIWindow* character1_win = nullptr;
	UIWindow* character2_win = nullptr;
	UIWindow* character3_win = nullptr;
	UIWindow* character4_win = nullptr;
	
};

#endif
