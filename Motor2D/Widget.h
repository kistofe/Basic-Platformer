#ifndef _WIDGET_
#define _WIDGET_

#include "p2Point.h"
#include "p2List.h"
#include "j1Module.h"


#include "SDL\include\SDL.h"


enum UiElemType
{
	LABEL,
	BUTTON,
	DYNAMIC_LABEL,
	IMAGE,
	WINDOW,
};

class Widget
{
public:


	Widget(UiElemType type, iPoint pos, j1Module* callback);

	virtual ~Widget();

	virtual bool PreUpdate(float d_time) { return true; };
	virtual bool Update(float d_time) { return true; };
	virtual void Draw() { return; };
	void Drag();


	void UpdateAttachedPositions();
	void Attach(Widget* widg_to_attach, const iPoint relative_pos);
public:

	UiElemType		type;
	bool			being_clicked;
	SDL_Rect		world_area;
	iPoint			position;

protected:

	iPoint		relative_position; // position relative to the element they are attached to
	j1Module*	callback = nullptr;
	p2List<Widget*> attached_widgets;

	bool attached;
	iPoint last_mousepos;
};
#endif 
