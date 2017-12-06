#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2Defs.h"
#include "j1EntityManager.h"
#include "j1GuiController.h"
#include <math.h>

struct SDL_Texture;
class Widget;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	virtual bool Start() { return true; };

	// Called each loop iteration
	virtual bool Update(float d_time) { return true; };

	// Called before all Updates
	virtual bool PostUpdate(float d_time) { return true; };

	// Called before quitting
	virtual bool CleanUp() { return true; }


public:

	Label* my_text = nullptr;
	Button* test_button = nullptr;
	Label* test_button_label = nullptr;

protected:

	SDL_Texture* background = nullptr;
};

#endif // __j1SCENE_H__