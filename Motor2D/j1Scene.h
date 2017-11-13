#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2Defs.h"
#include <math.h>

#define LAST_LVL 2

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float d_time);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void NextLevel();

private:

	bool LoadMapAtrib(pugi::xml_node&, p2SString*);
public:

	uint current_lvl = 1;
	fPoint gravity;
	fPoint max_gravity;

private:

	p2List<p2SString*> map_name;
	p2List<char*> map_music;
	SDL_Texture* debug_tex;
};

#endif // __j1SCENE_H__