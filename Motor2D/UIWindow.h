#ifndef _UIWINDOW_
#define _UIWINDOW_

#include "Widget.h"
#include "SDL\include\SDL.h"

class UIWindow : public Widget
{
public:
	UIWindow(iPoint pos, j1Module* callback);
	~UIWindow();

	void Draw();
	void SetArea();
	void SetWindowType(uint window_type);

public:

	SDL_Rect vertical_window;
	SDL_Rect horizontal_window;

private:
	SDL_Rect* atlas_section = nullptr;


};

#endif //_UIWINDOW_