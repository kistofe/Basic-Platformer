#ifndef _DYNAMICLABEL_
#define _DYNAMICLABEL_

#include "p2SString.h"
#include "Label.h"

#include "SDL\include\SDL.h"

class DynamicLabel : public Label
{
public:
	DynamicLabel(uint x, uint y);
	~DynamicLabel();

	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

private:
	p2SString* content;
};
#endif
