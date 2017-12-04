#ifndef _BUTTON_
#define _BUTTON_

#include "Widget.h"

#include "SDL/include/SDL.h"
class Label;

class Button : public Widget
{
public:
	Button(iPoint pos, j1Module* callback);
	~Button();

	bool PreUpdate(float d_time);
	void Draw();

	bool MouseOver(const SDL_Rect& button);
	void SetSection(SDL_Rect idle_sec, SDL_Rect high_sec, SDL_Rect clicked_sec);
	void CreateButtonLabel(Label* button_label, const char* content, SDL_Color color);

private:

	void ChangeVisualState(const int event);

private:
	bool hovering = false;

	Label* text = nullptr;
	SDL_Rect click_rect;
	SDL_Rect idle_rect;
	SDL_Rect highlighted_rect;
	SDL_Rect area;
	SDL_Rect* current_rect = nullptr;
	
};
#endif
