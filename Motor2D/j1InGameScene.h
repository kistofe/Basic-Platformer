#ifndef _INGAMESCENE_
#define _INGAMESCENE_

#include "j1Module.h"
#include "j1GuiController.h"

#include "SDL\include\SDL.h"

#define LAST_LVL 2

class j1InGameScene : public j1Module
{
public:
	j1InGameScene();
	~j1InGameScene();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float d_time);
	bool PostUpdate(float d_time);
	bool CleanUp();

	void NextLevel();
	void Initialize(const char* map_initialized);

private:

	void AddUiElems();
	void HandleInput();

public:

	//Main UI
	//DynamicLabel* life = nullptr;
	//DynamicLabel* time = nullptr;
	//DynamicLabel* score = nullptr;
	//DynamicLabel* coins = nullptr;

	//UI Textures
	SDL_Texture* life_tex = nullptr;
	SDL_Texture* time_tex = nullptr;
	SDL_Texture* coins_tex = nullptr;
	
	//Labels
	Label* curr_character = nullptr;
	Label* curr_level_lab = nullptr;

	//Scene Properties
	uint current_lvl = 1;
	fPoint gravity; 
	fPoint max_gravity; 
};
#endif
