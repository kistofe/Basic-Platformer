#include "j1App.h"
#include "DynamicLabel.h"
#include "j1Fonts.h"
#include "j1Textures.h"


DynamicLabel::DynamicLabel(iPoint pos, j1Module* callback) : Label(pos, callback)
{}


DynamicLabel::~DynamicLabel()
{
}

void DynamicLabel::ChangeContent(const char* new_content)
{
	if (text_texture != nullptr)
		App->tex->UnLoad(text_texture);

	text_texture = App->font->Print(new_content);
	int w, h;
	SDL_QueryTexture(text_texture, NULL, NULL, &w, &h);
	SetArea(w, h);
}

void DynamicLabel::SetArea(uint w, uint h)
{
	world_area.w = w;
	world_area.h = h;
}
