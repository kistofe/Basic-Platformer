#ifndef _LABEL_
#define _LABEL_

#include "Widgets.h"
#include "j1Fonts.h"

#include "SDL/include/SDL.h"

class Label : public Widgets
{
public:
	Label(uint x, uint y, j1Module* callback);
	~Label();

	
	void SetText(const char* content, const SDL_Color &color);
	void Draw();

public:
	SDL_Texture* text_texture;
};
#endif 
