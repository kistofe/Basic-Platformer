#include "j1App.h"
#include "DynamicLabel.h"
#include "j1Fonts.h"


DynamicLabel::DynamicLabel(iPoint pos, j1Module* callback) : Label(pos, callback)
{}


DynamicLabel::~DynamicLabel()
{
}

bool DynamicLabel::Update(float d_time)
{
	return true;
}

void DynamicLabel::ChangeContent(const char* new_content)
{
	text_texture = App->font->Print(new_content);
}
