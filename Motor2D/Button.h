#ifndef _BUTTON_
#define _BUTTON_

#include "Widget.h"

#include "SDL/include/SDL.h"
class Label;

enum ButtonType
{
	NEW_GAME = 0,
	LOAD_GAME,
	SETTINGS,
	CREDITS,
	EXIT,
	CLOSE_WINDOW,
	NONE
};

class Button : public Widget
{
public:
	Button(iPoint pos, j1Module* callback);
	~Button();

	bool PreUpdate(float d_time);
	void Draw();

	bool MouseOver(const SDL_Rect& button);
	void SetSection(SDL_Rect idle_sec, SDL_Rect high_sec, SDL_Rect clicked_sec);
	void SetButtonType(ButtonType type);

private:

	void ChangeVisualState(const int event);

public:

	ButtonType button_type;

private:
	bool hovering = false;

	SDL_Rect click_rect;
	SDL_Rect idle_rect;
	SDL_Rect highlighted_rect;
	SDL_Rect disabled;
	SDL_Rect* current_rect = nullptr;
	
};
#endif
