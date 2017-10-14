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

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	bool FadeToBlack(float time = 1.0f);

	bool SwitchMap(const char* map_on);
	bool IsFading();
	bool fading = false;

private:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

public:

private:

	uint start_time = 0;
	uint total_time = 0;
	SDL_Rect screen;
};


#endif // __j1SCENESWITCH_H__