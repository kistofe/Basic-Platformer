#include "j1App.h"
#include "Button.h"
#include "j1Input.h"
#include "Label.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1GuiController.h"

#include "SDL\include\SDL.h"


Button::Button(uint x, uint y, j1Module* callback) : Widgets(Widgets::UiElemType::BUTTON)
{
	this->callback = callback;
	position.x = x;
	position.y = y;
	current_rect = &idle_rect;
}


Button::~Button()
{
}

bool Button::Update()
{
	
	return true;
}


void Button::Draw()
{
	App->render->Blit(App->gui->GetAtlas(), position.x, position.y, current_rect);
}

bool Button::CheckClick(const SDL_Rect& button_area)
{
	bool ret = false;
	bool is_over = MouseOver(button_area);

	if (is_over && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT == KEY_DOWN))
		ret = true;

	return ret;
}

bool Button::MouseOver(const SDL_Rect& button)
{
	int x, y;
	App->input->GetMousePosition(x, y);

	return (x >= button.x && x <= button.x + button.w) && (y >= button.y && y <= button.y + button.h);
}

void Button::SetSection(SDL_Rect button_rec)
{
	idle_rect.x = button_rec.x;
	idle_rect.y = button_rec.y;
	idle_rect.w = button_rec.w;
	idle_rect.h = button_rec.h;
}

void Button::CreateButtonLabel(Label* button_label, const char* content, SDL_Color color)
{
	text = button_label;
	text->SetText(content, color);
}
