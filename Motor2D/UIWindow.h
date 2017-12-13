#ifndef _UIWINDOW_
#define _UIWINDOW_

#include "Widget.h"
#include "SDL\include\SDL.h"
enum WindowType
{
	HORIZONTAL_WINDOW,
	VERTICAL_WINDOW,
	TITLE_WINDOW
};

class UIWindow : public Widget
{
public:
	UIWindow(iPoint pos, j1Module* callback);
	~UIWindow();

	void Draw();
	void SetArea();
	void SetWindowType(WindowType type);
	void CloseWindow(UIWindow*);

public:

	SDL_Rect vertical_window;
	SDL_Rect horizontal_window;
	SDL_Rect title_window;
	
private:
	SDL_Rect* atlas_section = nullptr;


};

#endif //_UIWINDOW_