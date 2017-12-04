#ifndef _WIDGET_
#define _WIDGET_

#include "p2Point.h"
#include "j1Module.h"

enum UiElemType
{
	LABEL,
	BUTTON,
	DYNAMIC_LABEL,
	IMAGE,
};

class Widget
{
public:


	Widget(UiElemType type);

	virtual ~Widget();

	virtual bool PreUpdate(float d_time) { return true; };
	virtual bool Update(float d_time) { return true; };
	virtual void Draw() { return; };

public:

	UiElemType	type;

protected:

	iPoint		position;
	j1Module*	callback = nullptr;
};
#endif 
