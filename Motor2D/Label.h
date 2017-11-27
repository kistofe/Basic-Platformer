#ifndef _LABEL_
#define _LABEL_

#include "Widgets.h"

#include "SDL/include/SDL.h"

class Label : public Widgets
{
public:
	Label(uint x, uint y);
	~Label();

	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void Draw();

public:
	SDL_Texture* text;
};
#endif 
