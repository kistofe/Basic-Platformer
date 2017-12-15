#ifndef _INGAMESCENE_
#define _INGAMESCENE_

#include "j1Module.h"
#include "j1GuiController.h"
#include "j1Timer.h"

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
	bool PostUpdate();
	bool CleanUp();

//	bool Load(pugi::xml_node&);
//	bool Save(pugi::xml_node&) const;

	void NextLevel();
	void InitializeMap(const char* map_initialized);

private:

	void AddUiElems();
	void HandleInput();
	void UpdateUI();
	void UpdateTimer();
	void ResetTimer();
	void UpdateScore();
	void PauseGame();

public:

	//Main UI
	DynamicLabel* life = nullptr;
	DynamicLabel* time = nullptr;
	DynamicLabel* score = nullptr;
	DynamicLabel* coins = nullptr;
	DynamicLabel* level = nullptr;

	//UI Textures
	SDL_Texture* hud_tex = nullptr;

	//HUD elements
	SDL_Rect char1_life_icon;
	SDL_Rect char2_life_icon;
	SDL_Rect time_icon;
	SDL_Rect coins_icon;
	
	//Labels
	Label* curr_character = nullptr;
	Label* score_lab = nullptr;
	Label* tuto_window_content1 = nullptr;
	Label* tuto_window_content2 = nullptr;
	Label* tuto_window_content3 = nullptr;
	Label* tuto_window_content4 = nullptr;
	Label* pause_window_lab = nullptr;

	//Windows
	UIWindow* tuto_window = nullptr;
	UIWindow* pause_window = nullptr;
	UIWindow* pause_window_title = nullptr;

	//Scene Properties
	uint	timer_count = 0;
	uint	current_time;
	uint	max_time = 200;
	j1Timer	scene_timer;
	uint32	current_lvl = 1;
	fPoint	gravity; 
	fPoint	max_gravity; 
	bool	paused = false;
};
#endif
