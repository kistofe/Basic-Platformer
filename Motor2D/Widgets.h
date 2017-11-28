#ifndef _WIDGETS_
#define _WIDGETS_

#include "p2Point.h"

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
};
#endif // !_WIDGETS_
