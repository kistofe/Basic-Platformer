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

	// Called before quitting
	bool CleanUp();

	bool SwitchMap(const char* map_on);

public:

private:

	SDL_Rect screen;
};


#endif // __j1SCENESWITCH_H__