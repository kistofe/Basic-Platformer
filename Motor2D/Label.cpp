#include "Label.h"
#include "j1App.h"
#include "j1Fonts.h"


Label::Label(uint x, uint y, j1Module* callback) : Widgets(Widgets::UiElemType::LABEL)
{
	this->callback = callback;
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
