#ifndef _WIDGETS_
#define _WIDGETS_

#include "p2Point.h"
#include "j1Module.h"

enum UiElemType
{
	LABEL,
	BUTTON,
	DYNAMIC_LABEL,
	IMAGE,
};

class Widgets
{
public:

	Widgets();

	~Widgets();

	virtual bool Update() { return true; };
	virtual void Draw();

protected:

	iPoint		position;
	UiElemType	type;
	j1Module*	callback = nullptr;
};
#endif // !_WIDGETS_
