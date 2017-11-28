#include "Label.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"



Label::Label(uint x, uint y)
{
	position.create(x, y);
	text_texture = nullptr;
}


Label::~Label()
{
	if (text_texture != nullptr)
	App->tex->UnLoad(text_texture);
}

void Label::SetText(const char* content, const SDL_Color &color)
{
	text_texture = App->font->Print(content, color, App->font->default1);
}

void Label::Draw()
{
	App->render->Blit(text_texture, position.x, position.y);
}
