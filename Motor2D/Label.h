#ifndef _LABEL_
#define _LABEL_

#include "Widgets.h"

#include "SDL/include/SDL.h"

class Label : public Widgets
{
public:
	Label(uint x, uint y, j1Module* callback);
	~Label();

	bool CleanUp();

	void Draw();

public:
	SDL_Texture* text;
};
#endif 
