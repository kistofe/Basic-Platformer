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
private:

public:

private:
	SDL_Rect atlas_section;
};

#endif //_UIWINDOW_