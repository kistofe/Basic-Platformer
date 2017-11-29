#ifndef _BUTTON_
#define _BUTTON_

#include "Widgets.h"

#include "SDL/include/SDL.h"
class Label;

class Button : public Widgets
{
public:
	Button(uint x, uint y, j1Module* callback);
	~Button();

	bool Update();
	void Draw();

	bool CheckClick(const SDL_Rect& button);
	bool MouseOver(const SDL_Rect& button);
	void SetSection(SDL_Rect button_rec);
	void CreateButtonLabel(Label* button_label, const char* content, SDL_Color color);

private:

	Label* text = nullptr;
	SDL_Rect click_rect;
	SDL_Rect idle_rect;
	SDL_Rect highlighted_rect;
	SDL_Rect area;
	SDL_Rect* current_rect = nullptr;
	
};
#endif
