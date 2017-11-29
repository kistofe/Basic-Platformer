#ifndef __j1GUICONTROLLER_H__
#define __j1GUICONTROLLER_H__

#include "j1Module.h"
#include "Widgets.h"
#include "Button.h"
#include "Label.h"
#include "DynamicLabel.h"

#define CURSOR_WIDTH 2

// ---------------------------------------------------
class j1GuiController : public j1Module
{
public:

enum MouseEvents
{
	MOUSE_ENTER = 0,
	MOUSE_LEAVE,
	MOUSE_CLICK
};

	j1GuiController();

	// Destructor
	virtual ~j1GuiController();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	bool Update(float d_time);

	// Gui creation functions
	Widgets* CreateWidget(Widgets::UiElemType type, uint x, uint y, j1Module* callback = nullptr);
	bool DestroyWidget(Widgets* widget);

	SDL_Texture* GetAtlas() const;

private:
	p2List<Widgets*> ui_elems;
	SDL_Texture* atlas;
	p2SString atlas_file_name;
};

#endif // __j1GUI_H__