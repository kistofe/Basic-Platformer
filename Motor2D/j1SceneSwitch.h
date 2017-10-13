#ifndef __j1SCENESWITCH_H__
#define __j1SCENESWITCH_H__

#include "j1Module.h"

class j1SceneSwitch : public j1Module
{
public:

	j1SceneSwitch();

	// Destructor
	virtual ~j1SceneSwitch();

	// Called before the first frame
	bool Start();

	// Called before quitting
	bool CleanUp();

	bool FadeToBlack(Map* map_off, Map* map_on, float time = 1.0f);


};


#endif // __j1SCENESWITCH_H__