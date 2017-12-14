#ifndef _UIWINDOW_
#define _UIWINDOW_

#include "Widget.h"
#include "SDL\include\SDL.h"
enum WindowType
{
	HORIZONTAL_WINDOW,
	VERTICAL_WINDOW,
	HORIZONTAL_WINDOW_S,
	TITLE_WINDOW
};

class UIWindow : public Widget
{
public:
	UIWindow(iPoint pos, j1Module* callback);
	~UIWindow();

	void Draw();
	void SetWindowType(WindowType type);

private:
	void SetArea(uint w, uint h);

public:

	SDL_Rect vertical_window;
	SDL_Rect horizontal_window;
	SDL_Rect title_window;
	SDL_Rect horizontal_window_s;
	
private:
	SDL_Rect* atlas_section = nullptr;
	
};

#endif //_UIWINDOW_