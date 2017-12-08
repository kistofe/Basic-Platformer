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
	int wint = w, hint = h;
	world_area = { position.x, position.y, wint, hint };
}

void Label::SetText(const char* content, const SDL_Color &color)
{
	text_texture = App->font->Print(content, color, App->font->default1);
	int w = 0, h = 0;
	SDL_QueryTexture(text_texture, NULL, NULL, &w, &h);
	SetArea(w, h);
}

void Label::Draw()
{
	App->render->Blit(text_texture, position.x, position.y);
}
