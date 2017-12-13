#ifndef __j1SCENESWITCH_H__
#define __j1SCENESWITCH_H__

#include "j1Module.h"
#include "j1App.h"
#include "j1Window.h"
#include "SDL\include\SDL_rect.h"

class j1SceneSwitch : public j1Module
{
public:

	j1SceneSwitch();

	// Destructor
	virtual ~j1SceneSwitch();

	// Called before the first frame
	bool Start();

	bool Update(float d_time);

	bool SwitchMap(const char* map_on);

	bool DestroyEnemies();

	bool SwitchScene(j1Module* SceneIn, j1Module* SceneOut);

	bool IsSwitching() const;

public:

private:
	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	float fadetime = 1.0f;
	SDL_Rect screen;
	j1Module* to_enable = nullptr;
	j1Module* to_disable = nullptr;
	j1Timer switchtimer;
};


#endif // __j1SCENESWITCH_H__