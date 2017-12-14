#ifndef __j1GUICONTROLLER_H__
#define __j1GUICONTROLLER_H__

#include "j1Module.h"
#include "Widget.h"
#include "Button.h"
#include "Label.h"
#include "DynamicLabel.h"
#include "UIWindow.h"

#define CURSOR_WIDTH 2

// ---------------------------------------------------
enum MouseEvents
{
	MOUSE_ENTER = 0,
	MOUSE_LEAVE,
	MOUSE_CLICK,
	MOUSE_RELEASE
};

class j1GuiController : public j1Module
{
public:

		j1GuiController();

	// Destructor
	virtual ~j1GuiController();

	// Called when before render is available
	bool Awake(pugi::xml_node&);
	
	bool Start();
	bool PreUpdate(float d_time);
	bool Update(float d_time);
	bool CleanUp();

	// Gui creation functions
	Widget* CreateWidget(UiElemType type, uint x, uint y, j1Module* callback = nullptr);
	bool DestroyWidget(Widget* widget);

	SDL_Texture* GetAtlas() const;
	void Draw();

public:

	p2List<Widget*> ui_elems;
	bool dragging = false;

private:
	bool debug = false;
	SDL_Texture* atlas;
	p2SString atlas_file_name;
};

#endif // __j1GUI_H__