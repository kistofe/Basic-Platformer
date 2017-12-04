#ifndef _LABEL_
#define _LABEL_

#include "Widget.h"
#include "j1Fonts.h"

#include "SDL/include/SDL.h"

class Label : public Widget
{
public:
	Label(iPoint pos, j1Module* callback);
	~Label();

	
	void SetText(const char* content, const SDL_Color &color);
	void Draw();

public:
	SDL_Texture* text_texture;
};
#endif 
