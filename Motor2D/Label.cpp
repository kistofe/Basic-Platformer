#include "j1App.h"
#include "Label.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Fonts.h"

#include "p2Log.h"


Label::Label(iPoint pos, j1Module* callback) : Widget(UiElemType::LABEL, pos, callback)
{
}

Label::~Label()
{
	if (text_texture != nullptr)
		App->tex->UnLoad(text_texture);
}

void Label::SetArea(uint w, uint h)
{
	world_area.w = w;
	world_area.h = h;
}

bool Label::CleanUp()
{
	return true;
}

void Label::SetText(const char* content, const SDL_Color &color, _TTF_Font* font_size)
{
	text_texture = App->font->Print(content, color, font_size);
	int w, h;
	SDL_QueryTexture(text_texture, NULL, NULL, &w, &h);
	SetArea(w, h);
}

void Label::Draw()
{
	App->render->Blit(text_texture, position.x, position.y);
}
