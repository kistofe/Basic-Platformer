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
	void Drag();

	SDL_Rect GetArea();
private:

public:

private:
	SDL_Rect atlas_section;
	bool being_clicked = false;
	iPoint last_mousepos = { 0, 0 };
};

#endif //_UIWINDOW_