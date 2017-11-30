#ifndef _WIDGETS_
#define _WIDGETS_

#include "p2Point.h"
#include "j1Module.h"


class Widget
{
public:

enum UiElemType
{
	LABEL,
	BUTTON,
	DYNAMIC_LABEL,
	IMAGE,
};

	Widget(UiElemType type);

	~Widget();

	virtual bool PreUpdate(float d_time) { return true; };
	virtual bool Update(float d_time) { return true; };
	virtual void Draw() { return; };

protected:

	iPoint		position;
	UiElemType	type;
	j1Module*	callback = nullptr;
};
#endif // !_WIDGETS_
