#ifndef _DYNAMICLABEL_
#define _DYNAMICLABEL_

#include "p2SString.h"
#include "Label.h"

#include "SDL\include\SDL.h"

class DynamicLabel : public Label
{
public:
	DynamicLabel(iPoint pos, j1Module* callback);
	~DynamicLabel();

	void ChangeContent(const char* new_content);

private:
	void SetArea(uint w, uint h);

private:
	char* content;
};
#endif
