#ifndef _BUTTON_
#define _BUTTON_

#include "Widget.h"

#include "SDL/include/SDL.h"

enum ButtonType
{
	NEW_GAME = 0,
	LOAD_GAME,
	SETTINGS,
	M_VOLUME_UP,
	M_VOLUME_DOWN,
	S_VOLUME_UP,
	S_VOLUME_DOWN,
	TOGGLE_FULLSCREEN,
	CREDITS,
	EXIT,
	CLOSE_WINDOW,
	SELECT_RAMONA,
	SELECT_SCOTT,
	RESUME,
	TO_MAIN_SCENE,
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
	void SetSection(SDL_Rect idle_sec, SDL_Rect high_sec, SDL_Rect clicked_sec, SDL_Rect disabled_sec = { 0,0,0,0 });
	void SetButtonType(ButtonType type);
	void SetArea(uint w, uint h);

private:

	void ChangeVisualState(const int event);

public:

	ButtonType button_type;
	bool hovering = false;

private:
	
	bool enabled = true;
	uint click_sfx;

	SDL_Rect click_rect;
	SDL_Rect idle_rect;
	SDL_Rect highlighted_rect;
	SDL_Rect disabled_rect;
	SDL_Rect* current_rect = nullptr;
	
};
#endif
