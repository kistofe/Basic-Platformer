#ifndef _BUTTON_
#define _BUTTON_

#include "Widgets.h"

#include "SDL/include/SDL.h"
class Label;

class Button : public Widgets
{
public:
	Button(uint x, uint y);
	~Button();

	void Draw();

private:

	void OnClick();
	bool CheckClick(const SDL_Rect& button);
	bool MouseOver(const SDL_Rect& button);

private:
	SDL_Rect atlas_click_sect;
	SDL_Rect atlas_idle_sect;
	SDL_Rect atlas_highlighted_sect;

	Label* text;
};
#endif
