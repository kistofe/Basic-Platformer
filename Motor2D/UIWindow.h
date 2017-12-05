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
	SDL_Rect atlas_section;
	iPoint last_mousepos;
	bool being_clicked;
};

#endif //_UIWINDOW_